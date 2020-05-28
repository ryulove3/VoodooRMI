//
//  RMIBus.h
//  VoodooSMBus
//
//  Created by Avery Black on 4/30/20.
//  Copyright © 2020 leo-labs. All rights reserved.
//

#ifndef RMIBus_h
#define RMIBus_h

class RMIBus;
class RMIFunction;

#include <IOKit/IOLib.h>
#include <IOKit/IOService.h>
#include <IOKit/IOMessage.h>
#include "types.h"
#include "RMITransport.hpp"
#include "rmi.h"
#include "rmi_driver.hpp"

#include <F01.hpp>
#include <F11.hpp>
#include <F30.hpp>
#include <F34.hpp>

#define container_of(ptr, type, member) ({  \
    type *__mptr = (type *)(ptr);           \
    ((type *)(__mptr - offsetof(type, member))); })

#define IOLogError(arg...) IOLog("Error: " arg)
#define IOLogDebug(arg...) IOLog("Debug: " arg)

#define BIT(nr) (1UL << (nr))

enum {
    kHandleRMIInterrupt = iokit_vendor_specific_msg(1100)
};

class RMIBus : public IOService {
    OSDeclareDefaultStructors(RMIBus);
    
public:
    RMIBus * probe(IOService *provider, SInt32 *score) override;
    bool init(OSDictionary *dictionary) override;
    bool start(IOService *provider) override;
    void stop(IOService *provider) override;
    void free() override;
    
    rmi_driver_data *data;
    RMITransport *transport;
    
    // rmi_read
    inline int read(u16 addr, u8 *buf) {
        return transport->read(addr, buf);
    }
    // rmi_read_block
    inline int readBlock(u16 rmiaddr, u8 *databuff, size_t len) {
        return transport->readBlock(rmiaddr, databuff, len);
    }
    // rmi_write
    inline int write(u16 rmiaddr, u8 *buf) {
        return transport->write(rmiaddr, buf);
    }
    // rmi_block_write
    inline int blockWrite(u16 rmiaddr, u8 *buf, size_t len) {
        return transport->blockWrite(rmiaddr, buf, len);
    }
    
    OSSet *functions;
    
    inline IOReturn message(UInt32 type, IOService *provider, void *argument = 0) override;
    int rmi_register_function(rmi_function* fn);
    int rmi_smb_get_version();
private:
    void handleHostNotify();
};
    
#endif /* RMIBus_h */
