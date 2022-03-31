#ifndef __INCLUDE_HARDWARE_PCI_H
#define __INCLUDE_HARDWARE_PCI_H

#include <common/types.h>
#include <hardware/port.h>
#include <hardware/interrupts.h>
#include <drivers/driver.h>

namespace hardware {
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
      void SelectDrivers(drivers::DriverManager *driverManager);
      PeripheralComponentInterconnectDeviceDescription GetDeviceDescriptor(u16 bus, u16 device, u16 function);
  };
}

#endif