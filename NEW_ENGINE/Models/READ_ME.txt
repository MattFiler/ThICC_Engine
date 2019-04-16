This project when built will automatically comvert obj models placed in it into the required SDKMesh format.
See this wiki tutorial for more information aboutr the command line tool be used: 
https://github.com/Microsoft/DirectXTK12/wiki/Rendering-a-model 

1. rename the main model file something like _obj, as obj is a suffix used by VisStudio iteslef and thus may cause issues!
2. Put original files in to the required folder USING FILE EXPLORER, do not just drag and drop here.
2. Right click on Project Add -> Existing Item -> and then select the main obj file
3. Right click on the image now on the project  select Properties
4. Change the Item Type to "Custom Build Tool" and the press "APPLY"
5. You should now see options available for it
6. Copy over the same values from the exaple model into the boxes for your new file for "All Configurations" "All Platforms"