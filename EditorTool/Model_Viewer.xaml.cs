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
                ObjReader CurrentHelixObjReader = new ObjReader();
                Model3DGroup MyModel = CurrentHelixObjReader.Read(path_to_model);
                modelPreview.Content = MyModel;
            }
        }
    }
}
