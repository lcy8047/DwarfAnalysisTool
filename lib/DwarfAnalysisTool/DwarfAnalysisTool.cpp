#include <iostream>

#include "DwarfAnalysisTool.h"

void DwarfAnalysisTool::initSrcLineInfo( std::string file_path )
{
    Dwarf_Error error;
    Dwarf_Debug dbg;

    int res;

    res = dwarf_init_path( file_path.c_str(), nullptr, 0, DW_GROUPNUMBER_ANY, 
                    nullptr, nullptr, &dbg, &error );
    
    if ( res != DW_DLV_OK ) {
        std::cerr << "init path is failed" << std::endl;
        std::cerr << "errno : " << dwarf_errno( error ) << ", msg : " << dwarf_errmsg( error ) << std::endl;
        dwarf_dealloc_error( dbg, error );
        return ;
    }

    bool is_info = true;
    for ( int cu_count = 0; ; ++ cu_count ) {
        res = dwarf_next_cu_header_d(dbg, is_info, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, &error);
        if ( res == DW_DLV_NO_ENTRY ) {
            break;
        }
        if ( res == DW_DLV_ERROR ) {
            std::cerr << "next cu is failed" << std::endl;
            std::cerr << "errno : " << dwarf_errno( error ) << ", msg : " << dwarf_errmsg( error ) << std::endl;
            break;
        }
        Dwarf_Die cu_die = 0;

        res = dwarf_siblingof_b( dbg, 0, is_info, &cu_die, NULL );
        if ( res != DW_DLV_OK ) {
            std::cerr << "siblingof is failed" << std::endl;
            std::cerr << "errno : " << dwarf_errno( error ) << ", msg : " << dwarf_errmsg( error ) << std::endl;
            dwarf_dealloc_die( cu_die );
            continue;
        }

        Dwarf_Line_Context line_context;
        Dwarf_Unsigned version;
        Dwarf_Small table_count;
        res = dwarf_srclines_b( cu_die, &version, &table_count, &line_context, &error );
        if ( res != DW_DLV_OK ) {
            std::cerr << "srclines is failed" << std::endl;
            std::cerr << "errno : " << dwarf_errno( error ) << ", msg : " << dwarf_errmsg( error ) << std::endl;
            dwarf_srclines_dealloc_b( line_context );
            dwarf_dealloc_die( cu_die );
            break;
        }

        Dwarf_Line *line;
        Dwarf_Signed line_count; 
        res = dwarf_srclines_from_linecontext( line_context, &line, &line_count, &error );
        if ( res != DW_DLV_OK ) {
            std::cerr << "srclines is failed" << std::endl;
            std::cerr << "errno : " << dwarf_errno( error ) << ", msg : " << dwarf_errmsg( error ) << std::endl;
            dwarf_srclines_dealloc_b( line_context );
            dwarf_dealloc_die( cu_die );
            break;
        }
        
        for ( int i = 0; i < line_count; ++ i ) {
            Dwarf_Unsigned lineno;
            char *linesrc;
            Dwarf_Addr addr;
            char *name;
            bool is_ok = true;
            res = dwarf_lineno( line[i], &lineno, &error );
            if ( res != DW_DLV_OK ) {
                is_ok = false;
            }
            res = dwarf_linesrc( line[i], &linesrc, &error );
            if ( res != DW_DLV_OK ) {
                is_ok = false;
            }
            res = dwarf_lineaddr( line[i], &addr, &error );
            if ( res != DW_DLV_OK ) {
                is_ok = false;
            }
            res = dwarf_diename( cu_die, &name, &error );
            if ( res != DW_DLV_OK ) {
                is_ok = false;
            }
            if ( is_ok ) {
                struct SrcInfo s{ linesrc, lineno, name };
                _src_addr_table->emplace( addr, s );
            }
            
        }
        dwarf_srclines_dealloc_b( line_context );
        dwarf_dealloc_die( cu_die );
    }
    dwarf_dealloc_error( dbg, error );
    dwarf_finish( dbg );
}
