bl_info = {
    "name": "ThICC Blender Add-On",
    "category": "System",
    "author": "Matt Filer",
    "description": "Tools for developing custom maps in the ThICC Engine.",
}

import bpy
import json
import numpy as np
from bpy.types import Panel
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty


# The main UI panel for our tools
class ThICC_ToolPanel(Panel):
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'TOOLS'
    bl_label = 'ThICC Tools'
    bl_context = 'objectmode'
    bl_category = 'ThICC Tools'
    
    def draw(self,context):
        layout = self.layout
        layout.operator(ThICC_SpawnMarker.bl_idname, text='Set Spawn Position', icon='CURSOR')
        layout.operator(ThICC_TrackWaypoint.bl_idname, text='Add Track Waypoint', icon='CURSOR')
        layout.operator(ThICC_FinishLine.bl_idname, text='Add Finish Line', icon='CURSOR')
        layout.operator(ThICC_ItemBox.bl_idname, text='Add Item Box Location', icon='CURSOR')
        layout.operator(ThICC_GliderTrack.bl_idname, text='Add Glider Track', icon='MESH_PLANE')
        layout.operator(ThICC_CourseIntroCam.bl_idname, text='Add Course Intro Cam', icon='OUTLINER_OB_CAMERA')
        layout.operator(ThICC_ExportConfig.bl_idname, text='Save Config', icon='SAVE_COPY')
        
        
# Add a spawn marker (one per map)
class ThICC_SpawnMarker(bpy.types.Operator):
    """Add a New Spawn Marker"""
    bl_idname = "object.thicc_spawnmarker_add"
    bl_label = "Spawn Position"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        for item in bpy.context.selectable_objects:  
            item.select = False  
        
        #Create spawn spot object
        kart_spawn = bpy.data.objects.new(name="Kart Spawn Position", object_data=None)
        bpy.context.scene.objects.link(kart_spawn)
        kart_spawn.location = (0,0,0)
        
        #Set to selected object's position if we can
        if bpy.context.active_object != None:
            kart_spawn.location = bpy.context.scene.objects.active.location
            
        #Select object
        kart_spawn.select = True
        bpy.context.scene.objects.active = kart_spawn
        
        return {'FINISHED'}
        
        
# Add a track waypoint
class ThICC_TrackWaypoint(bpy.types.Operator):
    """Add a New Track Waypoint"""
    bl_idname = "object.thicc_trackwaypoint_add"
    bl_label = "Track Waypoint"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        for item in bpy.context.selectable_objects:  
            item.select = False  
            
        #Create plane
        waypoint_mesh = bpy.data.meshes.new("Plane")
        waypoint_object = bpy.data.objects.new("Plane", waypoint_mesh)
        waypoint_object.location = (0,0,0)
        waypoint_object.name = "Track Waypoint Marker"
        
        #Set to selected object's position if we can
        if bpy.context.active_object != None:
            waypoint_object.location = bpy.context.scene.objects.active.location
        
        #Put the plane in the scene
        bpy.context.scene.objects.link(waypoint_object)
        
        #Actually populate the plane
        waypoint_mesh.from_pydata([(0,0,0),(0,5,0),(0,5,5),(0,0,5)],[],[(0,1,2,3)])
        waypoint_mesh.update(calc_edges=True)
        
        #Select our new plane
        waypoint_object.select = True
        bpy.context.scene.objects.active = waypoint_object

        return {'FINISHED'}
        
        
# Add a marker for the finish line
class ThICC_FinishLine(bpy.types.Operator):
    """Add a Finish Line Marker"""
    bl_idname = "object.thicc_finishline_add"
    bl_label = "Finish Line"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        for item in bpy.context.selectable_objects:  
            item.select = False  
            
        #Create plane
        finishline_mesh = bpy.data.meshes.new("Plane")
        finishline_object = bpy.data.objects.new("Plane", finishline_mesh)
        finishline_object.location = (0,0,0)
        finishline_object.name = "Finish Line Marker"
        
        #Set to selected object's position if we can
        if bpy.context.active_object != None:
            finishline_object.location = bpy.context.scene.objects.active.location
        
        #Put the plane in the scene
        bpy.context.scene.objects.link(finishline_object)
        
        #Actually populate the plane
        finishline_mesh.from_pydata([(0,0,0),(0,5,0),(0,5,5),(0,0,5)],[],[(0,1,2,3)])
        finishline_mesh.update(calc_edges=True)
        
        #Select our new plane
        finishline_object.select = True
        bpy.context.scene.objects.active = finishline_object
        
        return {'FINISHED'}
        
        
# Add a glider track
class ThICC_GliderTrack(bpy.types.Operator):
    """Add a Glider Track Definition"""
    bl_idname = "object.thicc_glidertrack_add"
    bl_label = "Glider Track"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        for item in bpy.context.selectable_objects:  
            item.select = False  
            
        #Create plane
        glidertrack_mesh = bpy.data.meshes.new("Plane")
        glidertrack_object = bpy.data.objects.new("Plane", glidertrack_mesh)
        glidertrack_object.location = (0,0,0)
        glidertrack_object.name = "Glider Track"
        
        #Set to selected object's position if we can
        if bpy.context.active_object != None:
            glidertrack_object.location = bpy.context.scene.objects.active.location
        
        #Put the plane in the scene
        bpy.context.scene.objects.link(glidertrack_object)
        
        #Actually populate the plane
        glidertrack_mesh.from_pydata([(0,0,0),(0,5,0),(5,5,0),(5,0,0)],[],[(0,1,2,3)])
        glidertrack_mesh.update(calc_edges=True)
        
        #Select our new plane
        glidertrack_object.select = True
        bpy.context.scene.objects.active = glidertrack_object
        
        return {'FINISHED'}
        
        
# Add an item box location
class ThICC_ItemBox(bpy.types.Operator):
    """Add an Item Box Location"""
    bl_idname = "object.thicc_itembox_add"
    bl_label = "Item Box Spawn"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        for item in bpy.context.selectable_objects:  
            item.select = False  
            
        #Create itembox spawn spot
        itembox_spawn = bpy.data.objects.new(name="Item Box Spawn", object_data=None)
        bpy.context.scene.objects.link(itembox_spawn)
        itembox_spawn.location = (0,0,0)
        
        #Set to selected object's position if we can
        if bpy.context.active_object != None:
            itembox_spawn.location = bpy.context.scene.objects.active.location
            
        #Select it
        itembox_spawn.select = True
        bpy.context.scene.objects.active = itembox_spawn
        
        return {'FINISHED'}
        
        
# Add a cinematic camera start/end pos for course intro cutscenes
class ThICC_CourseIntroCam(bpy.types.Operator):
    """Add a New Course Intro Cinematic Camera"""
    bl_idname = "object.thicc_cinecam_add"
    bl_label = "Course Intro Camera"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        # Work out valid names
        camera_name_1 = "Course Intro Cam - Start 1"
        camera_name_2 = "Course Intro Cam - End 1"
        lookat_name = "Intro Cam Look-At - 1"
        camera_valid = True
        camera_update = 0
        location = (0,0,0)
        for object in bpy.data.objects:
            if camera_update < 1 and object.name == "Course Intro Cam - Start 1":
                camera_name_1 = "Course Intro Cam - Start 2"
                camera_name_2 = "Course Intro Cam - End 2"
                lookat_name = "Intro Cam Look-At - 2"
                camera_update = 1
            if camera_update < 2 and object.name == "Course Intro Cam - Start 2":
                camera_name_1 = "Course Intro Cam - Start 3"
                camera_name_2 = "Course Intro Cam - End 3"
                lookat_name = "Intro Cam Look-At - 3"
                camera_update = 2
            if camera_update < 3 and object.name == "Course Intro Cam - Start 3":
                camera_name_1 = "Course Intro Cam - Start 4"
                camera_name_2 = "Course Intro Cam - End 4"
                lookat_name = "Intro Cam Look-At - 4"
                camera_update = 3
            if camera_update < 4 and object.name == "Course Intro Cam - Start 4":
                camera_valid = False
                    
        # If we've reached the max cameras, don't add more
        if camera_valid == False:
            return {'CANCELLED'}
        
        # Get selected object's position if we can
        if bpy.context.active_object != None:
            location = bpy.context.scene.objects.active.location
            
        # Create look-at marker
        look_at = bpy.data.objects.new(name=lookat_name, object_data=None)
        bpy.context.scene.objects.link(look_at)
        look_at.location = location
        
        # Ceate camera 1 with calculated name
        bpy.ops.object.camera_add(location=location,rotation=[3.15/2,0,0])
        bpy.context.active_object.name = camera_name_1
        
        # Make camera 1 look at the look-at point
        look_at.select = True
        bpy.context.scene.objects.active = look_at
        bpy.ops.object.track_set(type = "TRACKTO")
        
        # Ceate camera 2 with calculated name
        bpy.ops.object.camera_add(location=location,rotation=[3.15/2,0,0])
        bpy.context.active_object.name = camera_name_2
        
        # Make camera 2 look at the look-at point
        look_at.select = True
        bpy.context.scene.objects.active = look_at
        bpy.ops.object.track_set(type = "TRACKTO")
        
        return {'FINISHED'}
    

# Save all config content
class ThICC_ExportConfig(bpy.types.Operator, ExportHelper):
    """Save ThICC Config"""
    bl_idname = "export_test.thicc_export"
    bl_label = "Save ThICC Config"
    
    filename_ext = ".json"

    filter_glob = StringProperty(
            default="*.json",
            options={'HIDDEN'},
            maxlen=255,  # Max internal buffer length, longer would be clamped.
            )

    def execute(self, context):
        #JSON template
        thicc_json = {'cams': [], 'look_at_points': [], 'spawns': [], 'waypoints': [], 'finish_line': [], 'item_boxes': [], 'glider_track': []}
        waypoint_index = 0
        
        for object in bpy.data.objects:
            if object.type == "EMPTY":
                #Kart spawn position
                if object.name[:19] == "Kart Spawn Position":
                    thicc_json["spawns"].append({
                        "name": object.name, 
                        "pos": [object.location[0], object.location[1], object.location[2]], 
                        "rotation": [object.rotation_euler[0], object.rotation_euler[1], object.rotation_euler[2]]
                    })
                #Item box position
                if object.name[:14] == "Item Box Spawn":
                    thicc_json["item_boxes"].append({
                        "pos": [object.location[0], object.location[1], object.location[2]], 
                        "rotation": [object.rotation_euler[0], object.rotation_euler[1], object.rotation_euler[2]]
                    })
                #Look-at position
                if object.name[:17] == "Intro Cam Look-At":
                    thicc_json["look_at_points"].append({
                        "index": object.name[-1:],
                        "pos": [object.location[0], object.location[1], object.location[2]]
                    })
                    
            if object.type == "CAMERA":
                #Cinematic cam 
                thicc_json["cams"].append({
                    "index": object.name[-1:],
                    "role": object.name[19:-2], 
                    "pos": [object.location[0], object.location[1], object.location[2]]
                })
                
            if object.type == "MESH":
                #Track waypoint
                if object.name[:21] == "Track Waypoint Marker":
                    if len(object.data.vertices) == 4:
                        top_left = object.matrix_world * object.data.vertices[2].co;
                        top_right = object.matrix_world * object.data.vertices[3].co;
                        bottom_left = object.matrix_world * object.data.vertices[0].co;
                        bottom_right = object.matrix_world * object.data.vertices[1].co;
                        thicc_json["waypoints"].append({
                            "index": waypoint_index,
                            "top_left": [top_left[0], top_left[1], top_left[2]],
                            "top_right": [top_right[0], top_right[1], top_right[2]],
                            "bottom_left": [bottom_left[0], bottom_left[1], bottom_left[2]],
                            "bottom_right": [bottom_right[0], bottom_right[1], bottom_right[2]]
                        })
                        waypoint_index += 1
                #Finish line marker
                if object.name[:18] == "Finish Line Marker":
                    if len(object.data.vertices) == 4:
                        top_left = object.matrix_world * object.data.vertices[2].co;
                        top_right = object.matrix_world * object.data.vertices[3].co;
                        bottom_left = object.matrix_world * object.data.vertices[0].co;
                        bottom_right = object.matrix_world * object.data.vertices[1].co;
                        thicc_json["finish_line"].append({
                            "top_left": [top_left[0], top_left[1], top_left[2]],
                            "top_right": [top_right[0], top_right[1], top_right[2]],
                            "bottom_left": [bottom_left[0], bottom_left[1], bottom_left[2]],
                            "bottom_right": [bottom_right[0], bottom_right[1], bottom_right[2]]
                        })
                #Glider track
                if object.name[:12] == "Glider Track":
                    if len(object.data.vertices) == 4:
                        top_left = object.matrix_world * object.data.vertices[2].co;
                        top_right = object.matrix_world * object.data.vertices[3].co;
                        bottom_left = object.matrix_world * object.data.vertices[0].co;
                        bottom_right = object.matrix_world * object.data.vertices[1].co;
                        thicc_json["glider_track"].append([
                            [top_left[0], top_left[1], top_left[2]], #Triangle1: top_left
                            [bottom_left[0], bottom_left[1], bottom_left[2]], #Triangle1: bottom_left
                            [bottom_right[0], bottom_right[1], bottom_right[2]] #Triangle1: bottom_right
                        ])
                        thicc_json["glider_track"].append([
                            [top_left[0], top_left[1], top_left[2]], #Triangle2: top_left
                            [top_right[0], top_right[1], top_right[2]], #Triangle2: top_right
                            [bottom_right[0], bottom_right[1], bottom_right[2]] #Triangle2: bottom_right
                        ])
        
        #Save out to file
        with open(self.filepath, 'w') as outfile:
            json.dump(thicc_json, outfile, indent=2, sort_keys=True)
        
        return {'FINISHED'}
    
    
# Add menu functions
def menu_func(self, context):
    self.layout.operator(ThICC_SpawnMarker.bl_idname)
    self.layout.operator(ThICC_TrackWaypoint.bl_idname)
    self.layout.operator(ThICC_CourseIntroCam.bl_idname)
    self.layout.operator(ThICC_ItemBox.bl_idname)
    self.layout.operator(ThICC_FinishLine.bl_idname)
    self.layout.operator(ThICC_GliderTrack.bl_idname)
    
def menu_func_export(self, context):
    self.layout.operator(ThICC_ExportConfig.bl_idname)
    
    
# Register plugin on load
def register():
    bpy.utils.register_class(ThICC_ToolPanel)
    bpy.utils.register_class(ThICC_SpawnMarker)
    bpy.utils.register_class(ThICC_TrackWaypoint)
    bpy.utils.register_class(ThICC_CourseIntroCam)
    bpy.utils.register_class(ThICC_ItemBox)
    bpy.utils.register_class(ThICC_FinishLine)
    bpy.utils.register_class(ThICC_GliderTrack)
    bpy.utils.register_class(ThICC_ExportConfig)
    bpy.types.VIEW3D_MT_object.append(menu_func)
    bpy.types.INFO_MT_file_export.append(menu_func_export)
    
# Unregister on exit
def unregister():
    bpy.utils.unregister_class(ThICC_ToolPanel)
    bpy.utils.unregister_class(ThICC_SpawnMarker)
    bpy.utils.unregister_class(ThICC_TrackWaypoint)
    bpy.utils.unregister_class(ThICC_CourseIntroCam)
    bpy.utils.unregister_class(ThICC_ItemBox)
    bpy.utils.unregister_class(ThICC_FinishLine)
    bpy.utils.unregister_class(ThICC_GliderTrack)
    bpy.utils.unregister_class(ThICC_ExportConfig)
    bpy.types.VIEW3D_MT_object.remove(menu_func)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)
    
    
if __name__ == '__main__':
    register()