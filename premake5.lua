dofile (path.join(os.getenv("DIR_SCRIPTS_ROOT"),"premake5_common.lua"))

workspace "NgoErr"

    SolutionConfiguration()
    defines {
        "NGO_ERR_USE_DYN"
    }
    local _exportSymbol = "NGO_ERR_MAKE_DLL"
    links { 
    }
    
    -- PROTECTED REGION ID(NgoErr.premake.solution) ENABLED START

    -- PROTECTED REGION END


project "NgoErr"

    PrefilterSharedLibBuildOptions("NgoErr")
    defines {_exportSymbol}
    
    -- PROTECTED REGION ID(NgoErr.premake.sharedlib) ENABLED START

    -- PROTECTED REGION END

    FilterSharedLibBuildOptions("NgoErr")


project "test_NgoErr"

    PrefilterTestBuildOptions("test_NgoErr")
    links { "NgoErr"}
    
    -- PROTECTED REGION ID(NgoErr.premake.test) ENABLED START

    -- PROTECTED REGION END

    FilterTestBuildOptions("test_NgoErr")
