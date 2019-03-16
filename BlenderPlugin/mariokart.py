bl_info = {
    "name": "Mario Kart Tools",
    "category": "System",
}

import bpy
import json
import numpy as np
from bpy.types import Panel


# The main UI panel for our Mario Kart tools
class MarioKartToolPanel(Panel):
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'TOOLS'
    bl_label = 'Mario Kart Tools'
    bl_context = 'objectmode'
    bl_category = 'Mario Kart'
    
    def draw(self,context):
        layout = self.layout
        layout.operator(MK_KartSpawnPos.bl_idname, text='Add Kart Spawn Position', icon='CURSOR')
        layout.operator(MK_TrackWaypoint.bl_idname, text='Add Track Waypoint', icon='CURSOR')
        layout.operator(MK_CourseIntroCam.bl_idname, text='Add Course Intro Cam', icon='OUTLINER_OB_CAMERA')
        layout.operator(MK_SaveAllIntroCams.bl_idname, text='TEST: Debug Output')
        
        
# Add a kart spawn position
class MK_KartSpawnPos(bpy.types.Operator):
    """Add a New Kart Spawn Position"""
    bl_idname = "object.mk_kartspawn_add"
    bl_label = "Kart Spawn Position"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        for item in bpy.context.selectable_objects:  
            item.select = False  
        
        kart_spawn = bpy.data.objects.new(name="Kart Spawn Position", object_data=None)
        bpy.context.scene.objects.link(kart_spawn)
        kart_spawn.location = bpy.context.scene.objects.active.location
        kart_spawn.select = True
        bpy.context.scene.objects.active = kart_spawn
        
        return {'FINISHED'}
        
        
# Add a track waypoint
class MK_TrackWaypoint(bpy.types.Operator):
    """Add a New Track Waypoint"""
    bl_idname = "object.mk_trackwaypoint_add"
    bl_label = "Track Waypoint"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        for item in bpy.context.selectable_objects:  
            item.select = False  
        
        track_waypoint = bpy.data.objects.new(name="Track Waypoint", object_data=None)
        bpy.context.scene.objects.link(track_waypoint)
        track_waypoint.location = bpy.context.scene.objects.active.location
        track_waypoint.select = True
        bpy.context.scene.objects.active = track_waypoint
        
        return {'FINISHED'}
        
        
# Add a cinematic camera start/end pos for course intro cutscenes
cinecam_types = [
    ("MK_CINECAM_1_START", "Course Intro Cam 1 - Start", "Course intro cam start position to animate from.", 1),
    ("MK_CINECAM_1_END", "Course Intro Cam 1 - End", "Course intro cam start position to animate to.", 2),
    ("MK_CINECAM_2_START", "Course Intro Cam 2 - Start", "Course intro cam start position to animate from.", 3),
    ("MK_CINECAM_2_END", "Course Intro Cam 2 - End", "Course intro cam start position to animate to.", 4),
    ("MK_CINECAM_3_START", "Course Intro Cam 3 - Start", "Course intro cam start position to animate from.", 5),
    ("MK_CINECAM_3_END", "Course Intro Cam 3 - End", "Course intro cam start position to animate to.", 6),
    ("MK_CINECAM_4_START", "Course Intro Cam 4 - Start", "Course intro cam start position to animate from.", 7),
    ("MK_CINECAM_4_END", "Course Intro Cam 4 - End", "Course intro cam start position to animate to.", 8),
    ]
class MK_CourseIntroCam(bpy.types.Operator):
    """Add a New Course Intro Cinematic Camera"""
    bl_idname = "object.mk_cinecam_add"
    bl_label = "Course Intro Camera"
    bl_options = {'REGISTER', 'UNDO'}

    #WIP FEATURE TO SELECT THE CAM'S ROLE (E.G. END CAM IN TWEEN OF INTRO CINEMATIC)
    cam_role = bpy.props.EnumProperty(name="Cam Role", items=cinecam_types)

    def execute(self, context):
        bpy.ops.object.camera_add(location=bpy.context.scene.objects.active.location,rotation=[3.15/2,0,0])
        bpy.context.active_object.name = "Course Intro Cam"
        #bpy.context.active_object["cam_role"] = cam_role
        
        return {'FINISHED'}
        
        
# Save all cinematic cams
class MK_SaveAllIntroCams(bpy.types.Operator):
    """Save All Cameras"""
    bl_idname = "object.mk_cinecam_get"
    bl_label = "SAVE ALL CINECAMS"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        mk_json = {'cams': [], 'spawns': [], 'waypoints': []}
        
        for object in bpy.data.objects:
            print(object.name)
            print(object.type)
            if object.type == "EMPTY":
                if object.name[:19] == "Kart Spawn Position":
                    mk_json["spawns"].append({"name": object.name, "pos": [object.location[0], object.location[1], object.location[2]], "rotation": [object.rotation_euler[0], object.rotation_euler[1], object.rotation_euler[2]]})
                if object.name[:14] == "Track Waypoint":
                    mk_json["waypoints"].append({"name": object.name, "pos": [object.location[0], object.location[1], object.location[2]]})
            if object.type == "CAMERA":
                mk_json["cams"].append({"name": object.name, "role": "WIP", "pos": [object.location[0], object.location[1], object.location[2]], "rotation": [object.rotation_euler[0], object.rotation_euler[1], object.rotation_euler[2]]})
        
        with open('D:/GEP-2019-WorkingAI/BlenderPlugin/MARIO_KART_JSON_DUMP.json', 'w') as outfile:
            json.dump(mk_json, outfile, indent=2, sort_keys=True)
        
        return {'FINISHED'}
    
    
# Add menu functions
def menu_func(self, context):
    self.layout.operator(MK_KartSpawnPos.bl_idname)
    self.layout.operator(MK_TrackWaypoint.bl_idname)
    self.layout.operator(MK_CourseIntroCam.bl_idname)
    self.layout.operator(MK_SaveAllIntroCams.bl_idname)
    
# Register plugin on load
def register():
    bpy.utils.register_class(MarioKartToolPanel)
    bpy.utils.register_class(MK_KartSpawnPos)
    bpy.utils.register_class(MK_TrackWaypoint)
    bpy.utils.register_class(MK_CourseIntroCam)
    bpy.utils.register_class(MK_SaveAllIntroCams)
    bpy.types.VIEW3D_MT_object.append(menu_func)
    
# Unregister on exit
def unregister():
    bpy.utils.unregister_class(MarioKartToolPanel)
    bpy.utils.unregister_class(MK_KartSpawnPos)
    bpy.utils.unregister_class(MK_TrackWaypoint)
    bpy.utils.unregister_class(MK_CourseIntroCam)
    bpy.utils.unregister_class(MK_SaveAllIntroCams)
    bpy.types.VIEW3D_MT_object.remove(menu_func)
    
    
if __name__ == '__main__':
    register()