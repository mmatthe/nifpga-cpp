#ifndef NIFPGA_CPP_H
#define NIFPGA_CPP_H

#include <string>

#include "NiFpga.h"

namespace nifpga {

  class fpga_exception : public std::runtime_error  {
  public:
    using runtime_error::runtime_error;
  };

  void throwIfError(NiFpga_Status status, const char* message) {
    if (NiFpga_IsError(status)) {
      throw fpga_exception("FPGA error. Error code: " + std::to_string(status) + "\n" + message);
    }
  }

  ///////////// readRegister
  struct control_tag {};
  struct indicator_tag {};

  template <class T>
  class Register {
  public:
    typedef T data_type;

    Register(unsigned int id, const std::string& name)
      : name(name), id(id) {}
    Register(unsigned int id)
      : name("Not Defined Register Name"), id(id) {}

    std::string name;
    unsigned int id;
  };


  template<typename T>
  struct always_false: std::false_type {};

  template <class data_type> NiFpga_Status readRegister_impl(NiFpga_Session session, uint32_t id, data_type* out) {
    static_assert(always_false<data_type>::value, "Non-Supported datatype for readRegister");
  }



#define readRegister_impl_instance(data_type, func)			\
  template <>								\
  NiFpga_Status readRegister_impl<data_type>(NiFpga_Session session, uint32_t id, data_type* out) { \
    return func(session, id, out);   }

  //////////// Write Register
  template <class data_type> NiFpga_Status writeRegister_impl(NiFpga_Session session, uint32_t id, data_type value);  // fallback implementation -> linker error for unkown data_types

#define writeRegister_impl_instance(data_type, func)			\
  template <>								\
  NiFpga_Status writeRegister_impl<data_type>(NiFpga_Session session, uint32_t id, data_type value) { \
    return func(session, id, value);  }



  ////////////// Instantiations
#define readWriteRegister_instance(data_type, typestr)			\
  readRegister_impl_instance(data_type, NiFpga_Read ## typestr);	\
  writeRegister_impl_instance(data_type, NiFpga_Write ## typestr);

  readWriteRegister_instance(uint8_t,  U8);
  readWriteRegister_instance(uint16_t, U16);
  readWriteRegister_instance(uint32_t, U32);
  readWriteRegister_instance(uint64_t, U64);
  readWriteRegister_instance(int8_t,   I8);
  readWriteRegister_instance(int16_t,  I16);
  readWriteRegister_instance(int32_t,  I32);
  readWriteRegister_instance(int64_t,  I64);


  template <class data_type>
  data_type readRegister(NiFpga_Session session, const Register<data_type>& reg) {
    data_type result;
    NiFpga_Status status = readRegister_impl(session, reg.id, &result);
    throwIfError(status, "readRegister");

    return result;
  }

/*  template <class data_type>
  data_type readRegister(NiFpga_Session session, const Register<data_type, control_tag>& reg) {
    static_assert(always_false<data_type>::value, "Attempting to read from a control register!");
  }*/


 /* template <class data_type>
  void writeRegister(NiFpga_Session session, const Register<data_type, indicator_tag>& reg, data_type value) {
    static_assert(always_false<data_type>::value, "Attempting to write to an indicator register!");
  }*/


  template <class data_type>
  void writeRegister(NiFpga_Session session, const Register<data_type>& reg, data_type value) {
    NiFpga_Status status = writeRegister_impl(session, reg.id, value);
    throwIfError(status, "writeRegister");
  }


  //////////// FIFOS
  struct target_to_host_tag;
  struct host_to_target_tag;
  template <class T>
  class Fifo {
  public:
    typedef T data_type;

    Fifo(unsigned int id, const std::string& name)
      : name(name), id(id) {}
    Fifo(unsigned int id)
      : name("Not Defined FIFO Name"), id(id) {}

    std::string name;
    unsigned int id;

  };

  template <class data_type>
  inline void configureFifo(NiFpga_Session session, const Fifo<data_type>& fifo, size_t requested_depth) {
    throwIfError(NiFpga_ConfigureFifo(session, fifo.id, requested_depth), "NiFpga_ConfigureFifo");
  }

  template <class data_type>
  inline void startFifo(NiFpga_Session session, const Fifo<data_type>& fifo) {
    throwIfError(NiFpga_StartFifo(session, fifo.id), "NiFpga_StartFifo");
  }

  template <class data_type>
  inline void stopFifo(NiFpga_Session session, const Fifo<data_type>& fifo) {
    throwIfError(NiFpga_StartFifo(session, fifo.id), "NiFpga_StopFifo");
  }

 template <class data_type>
  NiFpga_Status readFifo_impl(NiFpga_Session session, uint32_t id, data_type* buffer, size_t number_of_elements, uint32_t timeout, size_t* elements_remaining) {
    static_assert(always_false<data_type>::value, "Non-Supported data type for fifo reading!");
  }

  template <class data_type>
  NiFpga_Status writeFifo_impl(NiFpga_Session session, uint32_t id, data_type* buffer, size_t number_of_elements, uint32_t timeout, size_t* elements_remaining) {
    static_assert(always_false<data_type>::value, "Non-Supported data type for fifo writing!");
  }

#define readFifo_impl_instance(data_type, readfunc) \
  template <> \
  NiFpga_Status readFifo_impl<data_type>(NiFpga_Session session, uint32_t id, data_type* buffer, size_t number_of_elements, uint32_t timeout, size_t* elements_remaining) { \
    return readfunc(session, id, buffer, number_of_elements, timeout, elements_remaining); \
  }

#define writeFifo_impl_instance(data_type, writefunc) \
  template <> \
  NiFpga_Status writeFifo_impl<data_type>(NiFpga_Session session, uint32_t id, data_type* buffer, size_t number_of_elements, uint32_t timeout, size_t* elements_remaining) { \
    return writefunc(session, id, buffer, number_of_elements, timeout, elements_remaining); \
  }

#define readWriteFifo_impl_instance(data_type, typestr) \
  readFifo_impl_instance(data_type, NiFpga_ReadFifo ## typestr); \
  writeFifo_impl_instance(data_type, NiFpga_WriteFifo ## typestr);

  readWriteFifo_impl_instance(uint8_t,  U8);
  readWriteFifo_impl_instance(uint16_t, U16);
  readWriteFifo_impl_instance(uint32_t, U32);
  readWriteFifo_impl_instance(uint64_t, U64);
  readWriteFifo_impl_instance(int8_t,   I8);
  readWriteFifo_impl_instance(int16_t,  I16);
  readWriteFifo_impl_instance(int32_t,  I32);
  readWriteFifo_impl_instance(int64_t,  I64);


  template <class data_type>
  void readFifo(NiFpga_Session session, Fifo<data_type>& fifo, data_type* buffer_out, size_t number_of_elements, uint32_t timeout, size_t* elements_remaining) {
    throwIfError(readFifo_impl<data_type>(session, fifo.id, buffer_out, number_of_elements, timeout, elements_remaining), "readFifo");
  }
  /*template <class data_type>
  void readFifo(NiFpga_Session session, Fifo<data_type, host_to_target_tag>& fifo, data_type* buffer_out, size_t number_of_elements, uint32_t timeout, size_t* elements_remaining) {
    static_assert(always_false<data_type>::value, "Attempting to read from a host2target FIFO!");
  }*/

  template <class data_type>
  void writeFifo(NiFpga_Session session, Fifo<data_type>& fifo, data_type* buffer_out, size_t number_of_elements, uint32_t timeout, size_t* elements_remaining) {
    throwIfError(writeFifo_impl<data_type>(session, fifo.id, buffer_out, number_of_elements, timeout, elements_remaining), "writeFifo");
  }

  /*template <class data_type>
  void writeFifo(NiFpga_Session session, Fifo<data_type, target_to_host_tag>& fifo, data_type* buffer_out, size_t number_of_elements, uint32_t timeout, size_t* elements_remaining) {
    static_assert(always_false<data_type>::value, "Attempting to read from a target2host FIFO!");
  }*/


  //////////// General Functions
  inline void initialize() { throwIfError(NiFpga_Initialize(), "NiFpga_Initialize"); }
  inline void finalize() { throwIfError(NiFpga_Finalize(), "NiFpga_Finalize"); }

  inline NiFpga_Session open(const char* bitfile, const char* signature, const char* resource, uint32_t attribute) {
    NiFpga_Session session;
    throwIfError(NiFpga_Open(bitfile, signature, resource, attribute, &session), "NiFpga_Open");
    return session;
  }
  inline void close(NiFpga_Session session, uint32_t attribute) {
    throwIfError(NiFpga_Close(session, attribute), "NiFpga_Close");
  }

  inline void run(NiFpga_Session session, uint32_t attribute) { throwIfError(NiFpga_Run(session, attribute), "NiFpga_Run");  }
  inline void abort(NiFpga_Session session) { throwIfError(NiFpga_Abort(session), "NiFpga_Abort"); }
  inline void reset(NiFpga_Session session) { throwIfError(NiFpga_Reset(session), "NiFpga_Reset"); }
  inline void download(NiFpga_Session session) { throwIfError(NiFpga_Download(session), "NiFpga_Download"); }



} // end namespace

#endif /* NIFPGA-CPP_H */
