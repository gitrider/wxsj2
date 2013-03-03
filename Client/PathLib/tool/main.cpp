#include <iostream>
#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>

#include <io.h>
#include <direct.h>

#include "PathLibInterface.h"

typedef std::vector<std::string> FileNameList;
FileNameList gFileNameList; 

int main(int numargs, char** args)
{
    PathLib::PathLibRoot* pathLibRoot = new PathLib::PathLibRoot;

    PathLib::PathData* pathData = pathLibRoot->getPathData();

    if (numargs < 2)
    {
        std::cout << "no input file or dir,push any key to exit!";
        return -1;
    }    

    // 获取第二个参数，可能是文件名，也可能是文件夹
    char* args1 = args[1];

    long lHandle, res;
    struct _finddata_t tagData;

    lHandle = _findfirst(args1, &tagData);

    if (tagData.attrib & _A_SUBDIR)
    {
        if(chdir(args1) == -1)
        {
            std::cout << "change dir failed! " << args1 << " push any key to exit!" << std::endl;
            _findclose(lHandle);
            getchar();
            return -1;
        }

        lHandle = _findfirst("*.region", &tagData);
        res = 0;
        while (lHandle != -1 && res != -1)
        {
            if(!(tagData.attrib & _A_SUBDIR))
            {
                gFileNameList.push_back(std::string(args1) + "\\" + tagData.name);
            }
            res = _findnext( lHandle, &tagData );
        }
        // Close if we found any files
        if(lHandle != -1)
        {
            _findclose(lHandle);
        }

    }
    else if (tagData.attrib & _A_NORMAL || tagData.attrib & _A_RDONLY || tagData.attrib & _A_ARCH)
    {
        gFileNameList.push_back(args1);
    }
    else
    {
        std::cout << "unknown type of file or dir,push any key to exit!";
        _findclose(lHandle);
        return -1;
    }

    _findclose(lHandle);

    for (size_t fileNameIndex = 0; fileNameIndex < gFileNameList.size(); ++fileNameIndex)
    {
        const std::string& fileName = gFileNameList[fileNameIndex];

        std::string::size_type pos = fileName.find(".");

        if (pos != std::string::npos)
        {
            std::string fileNameNoExt = fileName.substr(0,pos);

            std::cout << fileNameNoExt << std::endl;

            if (pathData->getDataFromRegion(fileNameNoExt, 8))
            {
                pathData->exportPathFile(fileNameNoExt + ".path");

                pathData->importPathFile(fileNameNoExt + ".path");

                pathData->exportPathFileToXML(fileNameNoExt + ".path");
            }
            else
                std::cout << fileNameNoExt << " " << pathData->getErrorString() << std::endl;
        }
    }

    pathLibRoot->destroyPathData(pathData);
    delete pathLibRoot;

    std::cout << "export path file complete,push any key to exit!";
    getchar();
    return 0;

}