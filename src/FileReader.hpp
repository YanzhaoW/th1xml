#pragma once

#include <TFile.h>
#include <TH1.h>
#include <TKey.h>
#include <TList.h>
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;
class FileReader
{
  public:
    explicit FileReader(std::string filename)
        : m_FileName{ std::move(filename) }
        , fileHandle(TFile::Open(m_FileName.c_str(), "READ"))
        , m_FilePath{ m_FileName }
        , m_xmlDir{ m_FilePath.parent_path() }
    {
        m_BaseName = m_FilePath.stem().string();
    }

    template <typename RootType, typename = typename std::enable_if<std::is_base_of_v<TH1, RootType>>::type>
    void Read()
    {
        NewDirIfNeeded();
        ReadFromDir<RootType>(fileHandle);
    }

    auto SetOutputDir(const std::string& outputDir) { m_xmlDir = fs::directory_entry{ fs::path{ outputDir } }; }

    auto need_newDir(bool newdir) { fNewDir = newdir; }

    void ReadAll() { Read<TH1>(); }

  private:
    std::string m_FileName;
    std::string m_BaseName;
    TFile* fileHandle = nullptr;
    bool fNewDir = false;
    fs::path m_FilePath;
    fs::directory_entry m_xmlDir;

    template <typename RootType>
    void ReadFromDir(TDirectory* dir)
    {
        auto* keys = dir->GetListOfKeys();
        for (auto keyIter = keys->begin(); keyIter != keys->end(); ++keyIter)
        {
            auto* key = dynamic_cast<TKey*>(*keyIter);
            auto* obj = key->ReadObj();
            if (auto* hist = dynamic_cast<RootType*>(obj))
            {
                auto outputName = GetOutputName(hist, dir);
                // std::cout << outputName << std::endl;
                hist->SaveAs(outputName.c_str());
            }
            else if (auto* subdir = dynamic_cast<TDirectory*>(obj))
            {
                ReadFromDir<RootType>(subdir);
            }
        }
    }

    void NewDirIfNeeded()
    {
        if (!fNewDir)
        {
            return;
        }
        auto tempPath = m_xmlDir.path();
        (tempPath /= m_BaseName) /= "";
        m_xmlDir = fs::directory_entry{ tempPath };

        if (!m_xmlDir.exists())
        {
            fs::create_directory(m_xmlDir);
            std::cout << "creating new directory " << m_xmlDir.path().string() << "\n";
        }
        else
        {
            std::cout << "directory " << m_xmlDir.path().string() << " already exists"
                      << "\n";
        }
    }

    template <typename RootType>
    auto GetOutputName(RootType* hist, TDirectory* dir) -> std::string
    {
        return std::string{ m_xmlDir.path().string() + dir->GetName() } + "_" + std::string{ hist->GetName() } + ".xml";
    };
};
