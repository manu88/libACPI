/*
 * This file is part of the libACPI project
 * Copyright (c) 2018 Manuel Deneu.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include <iostream>
#include "AMLDecompilerInterface.hpp"


AMLDecompilerInterface::~AMLDecompilerInterface()
{}

AMLDecompilerInterface::AMLDecompilerInterface(AMLDecompiler &decomp):
decomp(decomp)
{
    decomp.userData = this;
    
    decomp.callbacks.onDefinitionBlock = []( AMLDecompiler* _decomp ,
                                            const ParserContext* context,
                                            const ACPIDefinitionBlock* desc)-> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->onACPIDefinitionBlock(context, *desc);
    };
    
    
    
    decomp.callbacks.startResourceTemplate = [](AMLDecompiler*decomp,  const ParserContext* context , size_t numItems ) ->int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(decomp->userData);
        
        return self->startResourceTemplate(context, numItems);
    };
    
    decomp.callbacks.endResourceTemplate = [] (AMLDecompiler*decomp, const ParserContext* context , size_t numItemsParsed, AMLParserError err) ->int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(decomp->userData);
        
        return self->endResourceTemplate(context, numItemsParsed, err);
    };
    
    decomp.callbacks.onBuffer = [](AMLDecompiler* _decomp , const ParserContext* ctx , size_t bufferSize , const uint8_t* buffer) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->onBuffer(ctx, bufferSize, buffer);
    };
    
    decomp.callbacks.startDevice = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIDevice* device) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->startDevice(context, *device);
    };
    
    decomp.callbacks.endDevice = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIDevice* dev) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->endDevice(context, *dev);
    };
    
    decomp.callbacks.startName = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* name) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->startName(context, name);
    };
    
    decomp.callbacks.onCreateField = [](AMLDecompiler* decomp, const ParserContext* context , const ACPICreateFieldBase *field) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(decomp->userData);
        
        return self->onCreateField(context, field);
    };
    /*
    decomp.callbacks.EndName = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* name) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->EndName(context, name);
    };
     */
    
    decomp.callbacks.startScope = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIScope* scope) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->startScope(context, *scope);
    };
    decomp.callbacks.endScope = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIScope* scope) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->endScope(context, *scope);
    };
    
    decomp.callbacks.onValue = []( AMLDecompiler* _decomp ,const ParserContext* context, uint64_t val) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->onValue(context, val);
    };
    
    decomp.callbacks.onString = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* str) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->onString(context, str);
    };

    decomp.callbacks.onOperationRegion = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIOperationRegion* reg) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->onOperationRegion(context, *reg);
    };
    
    
    decomp.callbacks.startIndexField = [] ( AMLDecompiler* _decomp , const ParserContext* context , const ACPIIndexField* indexField) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->startIndexField(context, *indexField);
    };
    
    decomp.callbacks.onIndexFieldElement = [] (AMLDecompiler* decomp,const ParserContext* context, const ACPIIndexFieldElement* fieldElement) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(decomp->userData);
        return self->onIndexFieldElement(context, *fieldElement);
    };
    
    decomp.callbacks.endIndexField = [] ( AMLDecompiler* _decomp , const ParserContext* context , const ACPIIndexField* indexField) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->endIndexField(context, *indexField);
    };
    
    decomp.callbacks.startField = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIField* field) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->startField(context, *field);
    };
    
    decomp.callbacks.onFieldElement = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIFieldElement* element) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->onFieldElement(context, *element);
    };
    
    decomp.callbacks.endField = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIField* field) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->endField(context, *field);
    };
    
    
    
    decomp.callbacks.onMethod = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIMethod* method) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->onMethod(context, *method);
    };
    /*
    decomp.callbacks.endMethod = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* location) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->endMethod(context, location);
    };
    */
    decomp.callbacks.onSmallItem = [] (AMLDecompiler*_decomp,const ParserContext* context,
                                       SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->onSmallItem(context, itemType, buffer, bufferSize);
    };
    
    decomp.callbacks.onLargeItem = [](AMLDecompiler*_decomp,const ParserContext* context, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->onLargeItem(context, itemType, buffer, bufferSize);
    };
    
    
    decomp.callbacks.startPackage = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIPackage* package) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->startPackage(context, *package);
    };
    
    decomp.callbacks.onPackageElement = []( AMLDecompiler* _decomp , const ParserContext* context , const ACPIPackageElement* element) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->onPackageElement(context, *element);
    };
    
    decomp.callbacks.endPackage = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIPackage* package) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->endPackage(context, *package);
    };
    
    
}

int AMLDecompilerInterface::onLargeItem(const ParserContext* context, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)
{
    switch (itemType)
    {
        case LargeResourceItemsType_MemoryRangeDescriptor32:
            assert(bufferSize == sizeof(MemoryRangeDescriptor32));
            return onMemoryRangeDescriptor32(context, reinterpret_cast<const MemoryRangeDescriptor32&>(*buffer));
            
        case LargeResourceItemsType_QWORDAddressSpaceDescriptor:
            return onQWORDAddressSpaceDescriptor(context, reinterpret_cast<const QWordAddressSpaceDescriptor&>(*buffer));
            
        case LargeResourceItemsType_WORDAddressSpaceDescriptor:
            return onWORDAddressSpaceDescriptor(context, reinterpret_cast<const WordAddressSpaceDescriptor&>(*buffer));
            
        case LargeResourceItemsType_DWORDAddressSpaceDescriptor:
            return onDWORDAddressSpaceDescriptor(context, reinterpret_cast<const DWordAddressSpaceDescriptor&>(*buffer));
        case LargeResourceItemsType_ExtendedIRQDescriptor:
            return onExtendedIRQDescriptor(context, reinterpret_cast<const ExtendedInterruptDescriptor&>(*buffer));
        default:
            assert(0);
            break;
    }
    return 0;
}

int AMLDecompilerInterface::onSmallItem(const ParserContext* context, SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)
{
    switch (itemType)
    {
        case SmallResourceItemsType_IOPortDescriptor:
            
            return onIOPortDescriptor(context, reinterpret_cast<const IOPortDescriptor&>(*buffer));
            
        case SmallResourceItemsType_IRQFormatDescriptor:
            return onIRQFormatDescriptor(context, reinterpret_cast<const IRQDescriptor&>(*buffer));
        default:
            std::cout << "unhandled small item " << itemType << std::endl;
            //assert(0);
            break;
    }
    return 0;
}
