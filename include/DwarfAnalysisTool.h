#ifndef DWARF_ANALYSIS_TOOL_H_
#define DWARF_ANALYSIS_TOOL_H_

#include <cstdint>
#include <libdwarf/libdwarf.h>
#include <string>
#include <map>

struct SrcInfo {
    std::string src_file;
    uint64_t line_num;
    std::string func_name;
};

class DwarfAnalysisTool {
public:
    DwarfAnalysisTool( void ) = delete;
    DwarfAnalysisTool( std::string bin_path )
        : _src_addr_table( new std::map<Dwarf_Addr, SrcInfo> )
    {
        initSrcLineInfo( bin_path );
    }
    ~DwarfAnalysisTool( void )
    {
        _src_addr_table->clear();
    }

    // GetResult.cpp
    void    GetSrc          ( uint64_t pc, std::string &src_file, uint64_t &src_lineno, std::string &src_line );
    void    PrintAll        ( bool print_dup_line=false, bool print_pc=false, bool print_full_path=false );
    void    PrintSourceLine ( uint64_t pc );

private:
    void    initSrcLineInfo ( std::string file_path );

    std::map<Dwarf_Addr, SrcInfo> *_src_addr_table;
};

#endif /* DWARF_ANALYSIS_TOOL_H_*/