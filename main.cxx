#include <FileReader.hpp>
#include <Option.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using std::runtime_error;

using th1xml::Option;

namespace po = boost::program_options;

auto main(int argc, char** argv) -> int
{

    auto desc = po::options_description{ "options for converting TH1 to xml file" };
    auto pos_desc = po::positional_options_description{};

    auto outputDir = Option<std::string>("outputDir,o");
    outputDir.Add(desc, "specify the output directory for xml files");

    auto rootFileName = Option<std::vector<std::string>>("root_filename");
    rootFileName.Add(desc, "root file names");
    rootFileName.MakePositional(pos_desc, -1);
    rootFileName.Set_required(true);

    auto has_dir_each = Option<bool>("dir_each,d", false);
    has_dir_each.Add(desc, "create a directory for each root file");

    auto help = Option<bool>("help,h");
    help.Add(desc, "help message");

    auto varMap = po::variables_map{};
    po::store(po::command_line_parser(argc, argv).positional(pos_desc).options(desc).run(), varMap);
    po::notify(varMap);

    if (varMap.count("help") != 0U)
    {
        std::cout << desc << std::endl;
        return 0;
    }
    try
    {
        has_dir_each.Retrieve(varMap);
        outputDir.Retrieve(varMap);
        rootFileName.Retrieve(varMap);
    }
    catch (const runtime_error& error)
    {
        std::cout << error.what() << std::endl;
    }

    for (auto const& name : rootFileName.Get())
    {
        auto reader = FileReader{ name };
        if (has_dir_each.Get())
        {
            reader.need_newDir(true);
        }
        if (!outputDir.Get().empty())
        {
            reader.SetOutputDir(outputDir.Get());
        }
        reader.ReadAll();
    }
    return 0;
}
