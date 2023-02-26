
dofile (path.join(os.getenv("DIR_SCRIPTS_ROOT"),"premake5_common.lua"))

-- solution NgoErr --
solution "NgoErr"

    SolutionConfiguration()

    -- COMMON CONFIGURATION MODIFICATION - START --
    configuration {}
        -- common defines (adapt if necessary) --
       defines {
                "NGO_ERR_USE_DYN"
               }                
       -- for shared libs, export statement
       local _exportSymbol = "NGO_ERR_MAKE_DLL"
       -- suffix to use for library versionning
       local _version = ""
       -- common libs  --
       links { 
          }
    -- COMMON CONFIGURATION MODIFICATION - END --

project "NgoErr"
    -- PROJECT MODIFICATIONS START--
    local _targetname = "NgoErr"
    -- additional defines --
    defines {_exportSymbol}
    -- PROJECT MODIFICATIONS END--

    AppendSharedLibBuildOptions(_targetname.._version)
