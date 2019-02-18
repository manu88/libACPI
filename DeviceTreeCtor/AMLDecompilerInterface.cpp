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

#include "AMLDecompilerInterface.hpp"


AMLDecompilerInterface::~AMLDecompilerInterface()
{}

AMLDecompilerInterface::AMLDecompilerInterface(AMLDecompiler &decomp):
decomp(decomp)
{
    decomp.userData = this;
    
    decomp.callbacks.OnDefinitionBlock = []( AMLDecompiler* _decomp ,
                                            const ParserContext* context,
                                            const ACPIDefinitionBlock* desc)-> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->onACPIDefinitionBlock(context, desc);
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
    
    decomp.callbacks.OnBuffer = [](AMLDecompiler* _decomp , const ParserContext* ctx , size_t bufferSize , const uint8_t* buffer) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->OnBuffer(ctx, bufferSize, buffer);
    };
    
    decomp.callbacks.StartDevice = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIDevice* device) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->StartDevice(context, device);
    };
    
    decomp.callbacks.EndDevice = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIDevice* dev) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->EndDevice(context, dev);
    };
    
    decomp.callbacks.StartName = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* name) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->StartName(context, name);
    };
    
    decomp.callbacks.EndName = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* name) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->EndName(context, name);
    };
    
    decomp.callbacks.StartScope = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* location) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->StartScope(context, location);
    };
    decomp.callbacks.EndScope = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* location) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->EndScope(context, location);
    };
    
    decomp.callbacks.OnValue = []( AMLDecompiler* _decomp ,const ParserContext* context, uint64_t val) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->OnValue(context, val);
    };

    decomp.callbacks.onOperationRegion = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIOperationRegion* reg) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->onOperationRegion(context, reg);
    };
    
    decomp.callbacks.onField = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIField* field) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->onField(context, field);
    };
    
    decomp.callbacks.startMethod = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* location) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->startMethod(context, location);
    };
    
    decomp.callbacks.endMethod = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* location) -> int
    {
        AMLDecompilerInterface* self = reinterpret_cast<AMLDecompilerInterface*>(_decomp->userData);
        return self->endMethod(context, location);
    };
    
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
    
    
}
