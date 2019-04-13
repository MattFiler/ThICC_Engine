﻿namespace EditorTool
{
    /* Asset types handled */
    public enum AssetType
    {
        MODEL,
        MESH,
        IMAGE,
        SOUND,
        FONT,
        STRING
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
}