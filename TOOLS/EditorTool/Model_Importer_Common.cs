using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EditorTool
{
    public enum importer_file
    {
        OBJ_MODEL,
        ENGINE_MESH,
        MATERIAL,
        CONFIG,
        METALLIC_CONFIG,
        COLLMAP,
        IMPORTER_CONFIG
    }

    public class Model_Importer_Common
    {
        UsefulFunctions function_libary = new UsefulFunctions();

        /* File Paths */

        private string import_directory = "";
        private string obj_file_name = "";
        private string sdkmesh_file_name = "";
        private string mtl_file_name = "";
        private string config_file_name = "";
        private string collmap_file_name = "";
        private string importer_config_file_name = "";
        private string metallic_config_file_name = "";
        private string asset_name = "";

        private bool did_set_paths = false;

        public void configureAssetPaths(string _filename)
        {
            asset_name = _filename;
            import_directory = function_libary.getFolder(AssetType.MODEL) + asset_name + "/";
            string file_prelude = import_directory + asset_name;
            obj_file_name = file_prelude + ".OBJ";
            sdkmesh_file_name = file_prelude + ".SDKMESH";
            mtl_file_name = file_prelude + ".MTL";
            config_file_name = file_prelude + ".JSON";
            collmap_file_name = file_prelude + ".COLLMAP";
            importer_config_file_name = import_directory + "IMPORTER_CONFIG.JSON";
            metallic_config_file_name = import_directory + "REFLECTION.ThICC";

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
                case importer_file.OBJ_MODEL:
                    return obj_file_name;
                case importer_file.ENGINE_MESH:
                    return sdkmesh_file_name;
                case importer_file.MATERIAL:
                    return mtl_file_name;
                case importer_file.CONFIG:
                    return config_file_name;
                case importer_file.COLLMAP:
                    return collmap_file_name;
                case importer_file.IMPORTER_CONFIG:
                    return importer_config_file_name;
                case importer_file.METALLIC_CONFIG:
                    return metallic_config_file_name;
            }
            return "";
        }
        public string modelName()
        {
            return asset_name;
        }

        //----
        /* Model Type */

        private ModelType selected_model_type;

        public void setModelType(ModelType _type)
        {
            selected_model_type = _type;
        }
        public ModelType getModelType()
        {
            return selected_model_type;
        }

        //----
        /* Edit or New Import? */

        private bool in_edit_mode = false;

        public void setEditMode(bool _editing)
        {
            in_edit_mode = _editing;
        }
        public bool getEditMode()
        {
            return in_edit_mode;
        }

        //----
        /* Path to Map Config */

        private string model_config_path = "";

        public void setModelConfigPath(string _path)
        {
            model_config_path = _path;
        }
        public string getModelConfigPath()
        {
            return model_config_path;
        }

        //----
        /* Fun Facts! */

        public struct FunFacts
        {
            public int material_count;
            public int collision_fix_count;
            public int vertices;
        };
        public FunFacts import_stats;

        //----
        /* Extra JSON */

        private string extra_json = "{ ";

        public void addToExtraJson(string _add)
        {
            extra_json += _add;
        }
        public string getExtraJson()
        {
            if (extra_json == "")
            {
                return extra_json;
            }
            return extra_json + " }";
        }
    }
}
