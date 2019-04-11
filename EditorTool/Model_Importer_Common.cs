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
        CONFIG,
        IMPORTER_CONFIG
    }

    public class Model_Importer_Common
    {
        private string import_directory = "";
        private string obj_file_name = "";
        private string mtl_file_name = "";
        private string config_file_name = "";
        private string importer_config_file_name = "";

        private bool did_set_paths = false;

        public void configureAssetPaths(string _filename)
        {
            import_directory = "DATA/MODELS/" + _filename + "/";
            obj_file_name = import_directory + _filename + ".obj";
            mtl_file_name = import_directory + _filename + ".mtl";
            config_file_name = import_directory + _filename + ".json";
            importer_config_file_name = import_directory + "importer_config.json";

            did_set_paths = true;
        }

        public string importDir()
        {
            return import_directory;
        }
        public string fileName(importer_file _file)
        {
            if (!did_set_paths)
            {
                throw new InvalidOperationException("Didn't set asset paths before calling them!");
            }
            switch (_file)
            {
                case importer_file.MODEL: 
                    return obj_file_name;
                case importer_file.MATERIAL:
                    return mtl_file_name;
                case importer_file.CONFIG:
                    return config_file_name;
                case importer_file.IMPORTER_CONFIG:
                    return importer_config_file_name;
            }
            return "";
        }

        //----

        private ModelType selected_model_type;

        public void setModelType(ModelType _type)
        {
            selected_model_type = _type;
        }
        public ModelType getModelType()
        {
            return selected_model_type;
        }
    }
}
