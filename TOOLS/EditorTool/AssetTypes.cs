namespace EditorTool
{
    /* Asset types handled */
    public enum AssetType
    {
        MODEL,
        //MESH, - now depreciated
        IMAGE,
        SOUND,
        FONT,
        STRING,
        CUBEMAP //new
    }
    /* Model types handled */
    public enum ModelType
    {
        MAP,
        ITEM,
        PLAYER,
        PROP
    }
    /* Modular collision types for models */
    public enum CollisionType
    {
        //NEW COLLISION TYPES SHOULD ALWAYS BE ADDED TO THE BOTTOM OF THE LIST ABOVE NUM_OF_TYPES!
        ON_TRACK,
        OFF_TRACK,
        BOOST_PAD,
        WALL,
        GLIDER_TRACK, //This should be ignored in any manual configurations, it's an auto-generated property.
        ANTIGRAV_PAD,
        JUMP_PAD,
        ON_TRACK_NO_AI,

        NUM_OF_TYPES //must always be last
    }
    /* Types of available asset compilations */
    public enum AssetCompType
    {
        MAP,
        CHARACTER,
        VEHICLE,
        ENGINE_FONT,
        GLOBAL_SOUNDS,
        CUP,
        GLOBAL_MODELS,
        GAMEPAD_GLPYHS
    }
}
