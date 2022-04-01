#ifndef __INCLUDE_HARDWARE_PCI_H
#define __INCLUDE_HARDWARE_PCI_H

#include <common/types.h>
#include <hardware/port.h>
#include <hardware/interrupts.h>
#include <drivers/driver.h>

namespace hardware {
  enum BaseAddressRegisterType {
    MemoryMapping = 0, 
    InputOutput = 1
  };

  class BaseAddressRegister {
    public:
      bool prefetchable;
      u8 *address;
      u32 size;
      BaseAddressRegisterType type;
  };

  class PeripheralComponentInterconnectDeviceDescription {
    public:
      u32 portBase;
      u32 interrupt;
      u16 bus;
      u16 device;
      u16 function;
      u16 vendorId;
      u16 deviceId;
      u8 classId;
      u8 subclassId;
      u8 interfaceId;
      u8 revision;
      PeripheralComponentInterconnectDeviceDescription();
      ~PeripheralComponentInterconnectDeviceDescription();
  };

  class PeripheralComponentInterconnectController {
    private:
      hardware::Port32Bit dataPort;
      hardware::Port32Bit commandPort;

    public:
      PeripheralComponentInterconnectController();
      ~PeripheralComponentInterconnectController();
      u32 Read(u16 bus, u16 device, u16 function, u32 registerOffset);
      void Write(u16 bus, u16 device, u16 function, u32 registerOffset, u32 value);
      bool DeviceHasFunctions(u16 bus, u16 device);
      drivers::Driver *GetDriver(PeripheralComponentInterconnectDeviceDescription device, InterruptManager *interrupts);
      void SelectDrivers(drivers::DriverManager *manager, InterruptManager *interrupts);
      PeripheralComponentInterconnectDeviceDescription GetDeviceDescriptor(u16 bus, u16 device, u16 function);
      BaseAddressRegister GetBaseAddressRegister(u16 bus, u16 device, u16 function, u16 bar);
  };
}

#endif