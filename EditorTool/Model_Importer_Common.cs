using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EditorTool
{
    public enum importer_file
    {
        MODEL,
        MATERIAL,
        CONFIG
    }

    public class Model_Importer_Common
    {
        private string import_directory = "DATA/MODELS/"; // + FOLDER + "/"
        private string obj_file_name = /*import_directory +*/ "model.obj";
        private string mtl_file_name = /*import_directory +*/ "model_materials.mtl";
        private string config_file_name = /*import_directory +*/ "model_config.json";

        public void configureAssetPaths(string _filename)
        {
            import_directory += _filename + "/";
            obj_file_name = import_directory + "model.obj";
            mtl_file_name = import_directory + "model_materials.mtl";
            config_file_name = import_directory + "model_config.json";
        }

        public string importDir()
        {
            return import_directory;
        }
        public string fileName(importer_file _file)
        {
            switch (_file)
            {
                case importer_file.MODEL: 
                    return obj_file_name;
                case importer_file.MATERIAL:
                    return mtl_file_name;
                case importer_file.CONFIG:
                    return config_file_name;
            }
            return "";
        }
    }
}
