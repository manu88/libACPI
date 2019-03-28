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

#pragma once

#include "ACPIObjects.hpp"
#include <vector>
#include <list>
#include <string>

struct TreeNode
{
    
    TreeNode():
    parent(nullptr)
    {}
    
    TreeNode( const std::string &id ,TreeNode*parent):
    id(id),
    parent(parent)
    {}
    
    TreeNode* getChildByName(const std::string&name)
    {
        auto c = std::count_if(children.begin(), children.end() , [name](const std::unique_ptr<TreeNode> &node)
                               {
                                   return node.get()->id == name;
                               });
        
        assert(c<=1);
        auto iter = std::find_if(children.begin(), children.end() , [name](const std::unique_ptr<TreeNode> &node)
                                 {
                                     return node.get()->id == name;
                                 });
        
        if (iter != children.end())
        {
            return iter->get();
        }
        return nullptr;
    }
    
    std::string id;
    
    TreeNode* parent;
    
    std::list< std::unique_ptr<TreeNode> > children;
    
    
    
    std::vector<ACPI::NameDeclaration>     _names;
    std::vector<ACPIOperationRegion> _opRegions;
    std::vector<ACPI::FieldDeclaration>    _fields;
    std::vector<ACPIMethod>          _methods;
};

struct DeviceTree
{
    TreeNode root;
    
    TreeNode* getNodeForPathAndCreateIfNeeded( const std::string &path , const std::string &relativeTo);
    TreeNode* getNodeForPath( const std::string &path , const std::string &relativeTo);
    
    
    ACPIDefinitionBlock defBlock = {0};
};

