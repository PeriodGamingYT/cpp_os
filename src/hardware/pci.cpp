#include <hardware/pci.h>
void printf(const char *);
void printfHex(u8);

using namespace hardware;

PeripheralComponentInterconnectDeviceDescription::PeripheralComponentInterconnectDeviceDescription() {}
PeripheralComponentInterconnectDeviceDescription::~PeripheralComponentInterconnectDeviceDescription() {}

PeripheralComponentInterconnectController::PeripheralComponentInterconnectController() 
  : dataPort(0xCFC),
    commandPort(0xCF8) {}
PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController() {}

u32 PeripheralComponentInterconnectController::Read(
  u16 bus, 
  u16 device, 
  u16 function, 
  u32 registerOffset) {
  u32 id = 
    (0x1 << 31) |
    ((bus & 0xFF) << 16) | 
    ((device & 0x1F) << 11) |
    ((function & 0x07) << 8) |
    (registerOffset & 0xFC);
  commandPort.Write(id);
  u32 result = dataPort.Read();
  return result >> (8 * (registerOffset % 4));
}

void PeripheralComponentInterconnectController::Write(
  u16 bus, 
  u16 device, 
  u16 function, 
  u32 registerOffset, 
  u32 value) {
  u32 id = 
    (0x1 << 31) |
    ((bus & 0xFF) << 16) | 
    ((device & 0x1F) << 11) |
    ((function & 0x07) << 8) |
    (registerOffset & 0xFC);
  commandPort.Write(id);
  dataPort.Write(value);
}

bool PeripheralComponentInterconnectController::DeviceHasFunctions(u16 bus, u16 device) {
  return Read(bus, device, 0, 0x0E) & (1 << 7);
}

void PeripheralComponentInterconnectController::SelectDrivers(drivers::DriverManager *driverManager) {
  for(u16 bus = 0; bus < 8; bus++) {
    for(u16 device = 0; device < 32; device++) {
      u16 numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
      for(u16 function = 0; function < numFunctions; function++) {
        PeripheralComponentInterconnectDeviceDescription deviceDescriptor = GetDeviceDescriptor(bus, device, function);
        if(deviceDescriptor.vendorId == 0x0000 ||
           deviceDescriptor.vendorId == 0xFFFF) {
          continue;
        }

        printf("PCI BUS: ");
        printfHex(bus & 0xFF);
        printf(", DEVICE: ");
        printfHex(device & 0xFF);
        printf(", FUNCTION: ");
        printfHex(bus & 0xFF);
        printf(" = VENDOR: ");
        printfHex((deviceDescriptor.vendorId & 0xFF00) >> 8);
        printf(", DEVICE: ");
        printfHex((deviceDescriptor.deviceId & 0xFF00) >> 8);
        printf(".\n");
      }
    }
  }
}

PeripheralComponentInterconnectDeviceDescription 
  PeripheralComponentInterconnectController::GetDeviceDescriptor(
    u16 bus, 
    u16 device, 
    u16 function) {
  PeripheralComponentInterconnectDeviceDescription result;
  result.bus = bus;
  result.device = device;
  result.function = function;
  result.vendorId = Read(bus, device, function, 0x00);
  result.deviceId = Read(bus, device, function, 0x02);
  result.classId = Read(bus, device, function, 0x0b);
  result.subclassId = Read(bus, device, function, 0x0a);
  result.interfaceId = Read(bus, device, function, 0x09);
  result.revision = Read(bus, device, function, 0x08);
  result.interrupt = Read(bus, device, function, 0x3c);
  return result;
}