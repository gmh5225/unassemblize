/**
 * @file
 *
 * @brief Class encapsulating the executable being disassembled.
 *
 * @copyright Unassemblize is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            3 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#include "executabletypes.h"
#include "functiontypes.h"
#include "pdbreadertypes.h"
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <stdio.h>

namespace LIEF
{
class Binary;
}

namespace unassemblize
{
class Executable
{
    using Address64ToIndexMap = std::unordered_map<Address64T, IndexT>;
    using StringToIndexMap = std::unordered_map<std::string, IndexT>;

public:
    Executable();
    ~Executable();

    void set_verbose(bool verbose) { m_verbose = verbose; }

    bool read(const std::string &exe_file);

    void load_config(const char *file_name, bool overwrite_symbols = false);
    void save_config(const char *file_name);

    bool is_ready() const;
    const std::string &get_filename() const;
    const ExeSections &get_sections() const;
    const ExeSectionInfo *find_section(uint64_t address) const;
    const ExeSectionInfo *find_section(const std::string &name) const;
    const ExeSectionInfo *get_code_section() const;
    uint64_t image_base() const; // Default image base address if the ASLR is not enabled.
    uint64_t code_section_begin_from_image_base() const; // Begin address of .text section plus image base.
    uint64_t code_section_end_from_image_base() const; // End address of .text section plus image base.
    uint64_t all_sections_begin_from_image_base() const; // Begin address of first section plus image base.
    uint64_t all_sections_end_from_image_base() const; // End address of last section plus image base.
    const ExeSymbol &get_symbol(uint64_t address) const;
    const ExeSymbol &get_symbol(const std::string &name) const;
    const ExeSymbol &get_symbol_from_image_base(uint64_t address) const; // Adds the image base before symbol lookup.
    const ExeSymbol &get_nearest_symbol(uint64_t address) const; // #TODO: investigate
    const ExeSymbols &get_symbols() const;

    /*
     * Adds series of new symbols if not already present.
     */
    void add_symbols(const ExeSymbols &symbols, bool overwrite = false);
    void add_symbols(const PdbSymbolInfoVector &symbols, bool overwrite = false);

    /*
     * Adds new symbol if not already present.
     */
    void add_symbol(const ExeSymbol &symbol, bool overwrite = false);

private:
    void load_symbols(nlohmann::json &js, bool overwrite_symbols);
    void dump_symbols(nlohmann::json &js) const;

    void load_sections(nlohmann::json &js);
    void dump_sections(nlohmann::json &js) const;

    void load_objects(nlohmann::json &js);
    void dump_objects(nlohmann::json &js) const;

private:
    std::string m_exeFilename;
    bool m_verbose = false;

    std::unique_ptr<LIEF::Binary> m_binary;

    ExeSections m_sections;
    StringToIndexMap m_sectionNameToIndexMap;
    IndexT m_codeSectionIdx = ~IndexT(0);

    ExeSymbols m_symbols;
    Address64ToIndexMap m_symbolAddressToIndexMap;
    StringToIndexMap m_symbolNameToIndexMap;

    ExeObjects m_targetObjects;

    ExeImageData m_imageData;

    static ExeSymbol s_emptySymbol;
};

} // namespace unassemblize