
dofile (path.join(os.getenv("DIR_SCRIPTS_ROOT"),"premake5_common.lua"))

workspace "NgoErr" 

    SolutionConfiguration()
    defines {
        "NGOERR_USE_DYN"
    }
    local _exportSymbol = "NGOERR_MAKE_DLL"
    links { 
    }
    
    -- PROTECTED REGION ID(NgoErr.premake.solution) ENABLED START
    -- Insert here user code

    -- End of user code
    -- PROTECTED REGION END

project "NgoErr"

    PrefilterSharedLibBuildOptions("NgoErr")
    defines {_exportSymbol}
    
   -- PROTECTED REGION ID(NgoErr.premake.sharedlib) ENABLED START
   -- Insert here user code

   -- End of user code
   -- PROTECTED REGION END

    FilterSharedLibBuildOptions("NgoErr")


    
