#Source files grouped by a directory
GET_SOURCE_FILES(SOURCES)

APPEND_SOURCE_FILES(SOURCES Curve)
APPEND_SOURCE_FILES(SOURCES InterpolationAlgorithms)
APPEND_SOURCE_FILES(SOURCES Integration)
APPEND_SOURCE_FILES(SOURCES LinAlg)
APPEND_SOURCE_FILES(SOURCES LinAlg/Dense)
APPEND_SOURCE_FILES(SOURCES ODE)
APPEND_SOURCE_FILES(SOURCES Nonlinear)

if(OGS_USE_EIGEN)
    APPEND_SOURCE_FILES(SOURCES LinAlg/Eigen)
endif()

if(OGS_USE_LIS)
    APPEND_SOURCE_FILES(SOURCES LinAlg/Lis)
    APPEND_SOURCE_FILES(SOURCES LinAlg/EigenLis)
endif()

if(OGS_USE_PETSC)
    APPEND_SOURCE_FILES(SOURCES LinAlg/PETSc)
endif()

# Create the library
add_library(MathLib ${SOURCES})

include(GenerateExportHeader)
generate_export_header(MathLib)
target_include_directories(MathLib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(MathLib PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(MathLib PUBLIC BaseLib logog)

if (CVODE_FOUND)
    target_link_libraries(MathLib PUBLIC ${CVODE_LIBRARIES})
endif()

if(LAPACK_FOUND)
    target_link_libraries(MathLib PUBLIC ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})
endif()

if (OGS_USE_LIS)
    target_link_libraries(MathLib PUBLIC ${LIS_LIBRARIES})
endif()

if (OGS_USE_MKL)
    target_link_libraries(MathLib PUBLIC ${MKL_LIBRARIES})
endif()

if (OGS_USE_PETSC)
    target_link_libraries(MathLib PUBLIC ${PETSC_LIBRARIES})
endif()

if(OGS_USE_PCH)
    cotire(MathLib)
endif()
