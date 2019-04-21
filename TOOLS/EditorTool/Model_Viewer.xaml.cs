using HelixToolkit.Wpf;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace EditorTool
{
    /// <summary>
    /// Interaction logic for ModelViewer.xaml
    /// </summary>
    public partial class ModelViewer : UserControl
    {
        public ModelViewer(string path_to_model = "")
        {
            InitializeComponent();

            //Load and display model
            if (File.Exists(path_to_model))
            {
                using (Stream model_loader = File.Open(path_to_model, FileMode.Open, FileAccess.Read))
                {
                    try
                    {
                        ObjReader CurrentHelixObjReader = new ObjReader();
                        Model3DGroup MyModel = CurrentHelixObjReader.Read(model_loader);
                        modelPreview.Content = MyModel;
                    }
                    catch { } //Will fail if model has MTL linked
                }
            }
        }
    }
}
