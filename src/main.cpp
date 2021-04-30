#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <stdio.h>

const uint32_t FILE_ENTRY_SIZE = 0x14;
//const uint32_t TREE_ENTRY_SIZE = 0x10;

uint32_t read_32LE(char* data)
{
    return (data[0] & 0xFF) | ((data[1] & 0xFF) << 8) | ((data[2] & 0xFF) << 16) | ((data[3] & 0xFF) << 24);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: BudgetRKV2Extractor \"path/to/the/archive.rkv\" \"output/directory/\"\n";
        return 0;
    }

    std::fstream in_file;
    in_file.open(argv[1], std::ios_base::in | std::ios_base::binary);
    in_file.seekg(0, in_file.end);
    uint32_t len = in_file.tellg();
    in_file.seekg(0, in_file.beg);
    char* file = new char[len];
    in_file.read(file, len);
    in_file.close();

    if (read_32LE(&file[0]) != 0x32564B52)
    {
        printf("File is not an RKV2 file\n");
        delete[] file;
        return 1;
    }

    uint32_t file_count = read_32LE(&file[0x04]);
    uint32_t file_metadata_pos = read_32LE(&file[0x14]);

    printf("File Size: 0x%.8X\nFile Count: %u\nFile Table Start: 0x%.8X\nFile Name Start: 0x%.8X\n", len, file_count, file_metadata_pos, file_metadata_pos + (file_count * FILE_ENTRY_SIZE) + 1);

    /*uint32_t file_path_pos = len - 1;

    while (file[file_path_pos] == 0x00)
    {
        file_path_pos--;
    }

    for (uint32_t i = 0; i < file_count; i++)
    {
        while (file[file_path_pos] != 0x00)
        {
            file_path_pos--;
        }

        if (file[file_path_pos - 1] == 0x00)
        {
            file_path_pos--;
            break;
        }

        file_path_pos--;
    }

    file_path_pos += 2;

    printf("Start of File Paths: %.8X\n", file_path_pos);
    */

    std::string base_path(argv[2]);

    std::string out_path = "";
    //std::string dir_temp = "";
    //std::string directories = "";
    std::string file_name = "";
    uint32_t curr_entry_pointer = file_metadata_pos;
    uint32_t file_name_pointer = file_metadata_pos + (file_count * FILE_ENTRY_SIZE) + 1;
    //uint32_t last_string_pos = file_path_pos;
    //uint32_t curr_string_pos = file_path_pos;
    uint32_t file_size = 0;
    uint32_t file_data_pointer = 0;

    std::fstream out_file;

    std::filesystem::create_directories(base_path);

    for (uint32_t j = 0; j < file_count; j++)
    {
        file_size = read_32LE(&file[curr_entry_pointer + 0x08]);
        file_data_pointer = read_32LE(&file[curr_entry_pointer + 0x0C]);

        file_name = "";
        //directories = base_path;
        out_path = base_path;
        //dir_temp = "";

        // get file name
        while (file[file_name_pointer] != 0x00)
        {
            file_name.append(1, file[file_name_pointer]);
            file_name_pointer++;
        }

        /*
        // get directory name
        while (file[curr_string_pos] != 0x00)
        {
            dir_temp.append(1, file[curr_string_pos]);

            if (file[curr_string_pos] == '\\')
            {
                directories.append(dir_temp);
                dir_temp = "";
            }

            curr_string_pos++;
        }

        printf("temp: '%s' -- file_name: '%s'\n", dir_temp.c_str(), file_name.c_str());
        if (dir_temp.compare(file_name) == 0)
        {
            printf("match!\n");
            std::filesystem::create_directories(directories);
            out_path.append(directories);
            out_path.append(file_name);
            curr_string_pos++;
            last_string_pos = curr_string_pos;
        }
        else
        {
            out_path.append(file_name);
            curr_string_pos = last_string_pos;
        }
        */

        out_path.append(file_name);

        out_file.open(out_path, std::ios_base::out | std::ios_base::binary);
        out_file.write(&file[file_data_pointer], file_size);
        out_file.close();

        curr_entry_pointer += FILE_ENTRY_SIZE;
        file_name_pointer++;
    }

    delete[] file;

    return 0;
}
