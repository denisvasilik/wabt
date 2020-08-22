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

#ifndef WABT_WASM_MODULE_H_
#define WABT_WASM_MODULE_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>
#include <map>

#include "src/common.h"
#include "src/feature.h"
#include "src/opcode.h"
#include "src/stream.h"
#include "src/ir.h"
#include "src/binary.h"

#define WASM_MODULE_IMPORT_KIND_SIZE 0x1

namespace wabt {

class SectionSizeDecorator {

    virtual std::size_t size() = 0;
};

class ImportSizeDecorator : public SectionSizeDecorator {
public:
    ImportSizeDecorator(const wabt::Module* module, wabt::Import* import);

    virtual std::size_t size();

protected:
    wabt::Index GetImportSignatureIndex();

private:
    const wabt::Module* module_;
    wabt::Import* import_;

    std::map<wabt::BinarySection, void*> sections_;
};

std::size_t GetSectionSize(const wabt::Module* module,
                           wabt::BinarySection section_code);

std::size_t GetImportSectionSize(const wabt::Module* module);

}  // namespace wabt

#endif /* WABT_WASM_MODULE_H_ */
