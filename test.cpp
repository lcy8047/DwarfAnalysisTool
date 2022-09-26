#include <iostream>
#include <elf.h>
#include <unistd.h>

#include "DwarfAnalysisTool.h"

int main( int argc, char *argv[] )
{
    std::string bin_path;
    while ( true ) {
        switch ( getopt( argc, argv, "b:" ) )
        {
            case 'b':
            {
                bin_path = optarg;
                continue;
            }
            default:
            {
                break;
            }
        }
        break;
    }
    if ( bin_path.empty() ) {
        std::cout << "binary path is empty" << std::endl;
        std::cout << "usage : " << argv[0] << " -b binary_file_name" << std::endl;
        return -1;
    }

    DwarfAnalysisTool *dat = new DwarfAnalysisTool( bin_path );
    dat->PrintAll();
    return 0;
}