//
//  ObjC-interface.h
//  VulkanTesting
//
//  Created by Andrés Ragot on 24/10/25.
//

#ifndef ObjC_interface_h
#define ObjC_interface_h

extern "C" {
    void * getViewFromNSWindowPointer(void * NSWindow);
    void makeViewMetalCompatible(void * handle);
}

#endif /* ObjC_interface_h */
