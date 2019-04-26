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
        ON_TRACK,
        OFF_TRACK,
        BOOST_PAD,
        WALL,

        NUM_OF_TYPES //must be last
    }
    /* Types of available asset compilations */
    public enum AssetCompType
    {
        MAP,
        CHARACTER,
        VEHICLE,
        ENGINE_FONT,
        GLOBAL_SOUNDS
    }
}
