#include <fstream>
#include <iomanip>
#include <iostream>

#include "DwarfAnalysisTool.h"

void DwarfAnalysisTool::GetSrc( uint64_t pc, std::string &src_file, uint64_t &src_lineno, std::string &src_line )
{
    
    if ( _src_addr_table->find( pc ) == _src_addr_table->end() ) {
        return;
    }

    struct SrcInfo src_info = _src_addr_table->at( pc );

    std::ifstream code_file_stream( src_info.src_file );
    std::string code_line;
    for ( uint64_t i = 1; i <= src_info.line_num; ++ i ) {
        getline( code_file_stream, code_line );
    }
    code_file_stream.close();

    src_file = src_info.src_file;
    src_lineno = src_info.line_num;
    src_line = code_line;
}

void DwarfAnalysisTool::PrintAll( bool print_dup_line, bool print_pc, bool print_full_path )
{
    uint64_t prev_line = 0;
    for ( auto it : *_src_addr_table ) {
        uint64_t pc = it.first;
        std::string src_path;
        uint64_t src_lineno;
        std::string src_line;
        GetSrc( pc, src_path, src_lineno, src_line );
        if ( ! print_full_path ) {
            src_path = src_path.substr( src_path.rfind('/') + 1 );
        }
        if ( print_pc ) {
            std::cout << std::hex << "0x" << pc << " : " << std::dec;
        }
        if ( ! print_dup_line && src_lineno == prev_line ) {
            continue;
        }
        std::cout << std::left << std::setw(82) << src_line << src_path << ":" << src_lineno << std::endl;
        prev_line = src_lineno;
    }
}

void DwarfAnalysisTool::PrintSourceLine( uint64_t pc )
{

    if ( _src_addr_table->find( pc ) == _src_addr_table->end() ) {
        return;
    }

    struct SrcInfo src_info = _src_addr_table->at( pc );

    std::ifstream code_file_stream( src_info.src_file );
    std::string code_line;
    for ( uint64_t i = 1; i <= src_info.line_num; ++ i ) {
        getline( code_file_stream, code_line );
    }
    code_file_stream.close();
    
    for( size_t i = 0; i < code_line.length(); ++ i ) {
        if ( code_line[i] == '\t' ) {
            code_line.replace( i, 1, "    " );
        }
    }

    while( code_line.back() == ' ' ) {
        code_line.pop_back();
    }
    std::cout << std::left << std::setw(82) << code_line << src_info.src_file << ":" << src_info.line_num << std::endl;
}