/*
 * Copyright 2016 WebAssembly Community Group participants
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "src/wasm-module.h"

#include <cassert>
#include <cmath>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "config.h"

#include "src/binary.h"
#include "src/cast.h"
#include "src/ir.h"
#include "src/leb128.h"
#include "src/stream.h"
#include "src/string-view.h"

namespace wabt {

ImportSizeDecorator::ImportSizeDecorator(
        const wabt::Module* module,
        wabt::Import* import) :
    import_(import),
    module_(module)
{
    sections_.insert(std::pair<wabt::BinarySection, void*>(
        wabt::BinarySection::Type, (void*)&(module_->types)));
    sections_.insert(std::pair<wabt::BinarySection, void*>(
        wabt::BinarySection::Import, (void*)&(module_->imports)));
    sections_.insert(std::pair<wabt::BinarySection, void*>(
        wabt::BinarySection::Function, (void*)&(module_->funcs)));
    sections_.insert(std::pair<wabt::BinarySection, void*>(
        wabt::BinarySection::Table, (void*)&(module_->tables)));
    sections_.insert(std::pair<wabt::BinarySection, void*>(
        wabt::BinarySection::Memory, (void*)&(module_->memories)));
    sections_.insert(std::pair<wabt::BinarySection, void*>(
        wabt::BinarySection::Event, (void*)&(module_->events)));
    sections_.insert(std::pair<wabt::BinarySection, void*>(
        wabt::BinarySection::Global, (void*)&(module_->globals)));
    sections_.insert(std::pair<wabt::BinarySection, void*>(
        wabt::BinarySection::Export, (void*)&(module_->exports)));
    sections_.insert(std::pair<wabt::BinarySection, void*>(
        wabt::BinarySection::Start, (void*)&(module_->starts)));
    sections_.insert(std::pair<wabt::BinarySection, void*>(
        wabt::BinarySection::Elem, (void*)&(module_->elem_segments)));
    sections_.insert(std::pair<wabt::BinarySection, void*>(
        wabt::BinarySection::Data, (void*)&(module_->data_segments)));
}

std::size_t ImportSizeDecorator::size() {
    return U32Leb128Length(import_->module_name.length()) +
           import_->module_name.length() +
           U32Leb128Length(import_->field_name.length()) +
           import_->field_name.length() +
           WASM_MODULE_IMPORT_KIND_SIZE +
           U32Leb128Length(GetImportSignatureIndex());
}

wabt::Index ImportSizeDecorator::GetImportSignatureIndex() {
    return module_->GetFuncTypeIndex(cast<FuncImport>(import_)->func.decl);
}

std::size_t GetSectionSize(const wabt::Module* module,
                           wabt::BinarySection section_code) {
    if(section_code == BinarySection::Import) {
        return GetImportSectionSize(module);
    }
}

std::size_t GetImportSectionSize(const wabt::Module* module) {
    auto section_size = 0;

    std::for_each(module->imports.begin(), module->imports.end(), [&module, &section_size](wabt::Import* import){
        section_size += ImportSizeDecorator(module, import).size();
    });

    return U32Leb128Length(module->imports.size()) + section_size;
}

namespace {

}  // end anonymous namespace

}  // namespace wabt