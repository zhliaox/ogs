/**
 * \copyright
 * Copyright (c) 2012-2016, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 */

#ifndef PROCESSLIB_UTILS_CREATE_LOCAL_ASSEMBLERS_H_
#define PROCESSLIB_UTILS_CREATE_LOCAL_ASSEMBLERS_H_

#include <vector>
#include <logog/include/logog.hpp>

#include "AssemblerLib/LocalToGlobalIndexMap.h"

#include "LocalDataInitializer.h"


namespace ProcessLib
{

namespace detail
{

template<unsigned GlobalDim, typename GlobalSetup,
         template <typename, typename, typename, typename, unsigned> class
         LocalAssemblerImplementation,
         typename LocalAssemblerInterface,
         typename... ExtraCtorArgs>
void createLocalAssemblers(
        AssemblerLib::LocalToGlobalIndexMap const& dof_table,
        std::vector<MeshLib::Element*> const& mesh_elements,
        unsigned const integration_order,
        std::vector<std::unique_ptr<LocalAssemblerInterface>>& local_assemblers,
        ExtraCtorArgs&&... extra_ctor_args
        )
{
    // Shape matrices initializer
    using LocalDataInitializer = LocalDataInitializer<
        LocalAssemblerInterface,
        LocalAssemblerImplementation,
        typename GlobalSetup::MatrixType,
        typename GlobalSetup::VectorType,
        GlobalDim,
        ExtraCtorArgs...>;

    DBUG("Create local assemblers.");
    // Populate the vector of local assemblers.
    local_assemblers.resize(mesh_elements.size());

    LocalDataInitializer initializer(dof_table);

    DBUG("Calling local assembler builder for all mesh elements.");
    GlobalSetup::transformDereferenced(
            initializer,
            mesh_elements,
            local_assemblers,
            integration_order,
            std::forward<ExtraCtorArgs>(extra_ctor_args)...);
}

} // namespace detail


/*! Creates local assemblers for each element of the given \c mesh.
 *
 * \tparam GlobalSetup the global setup of the process
 * \tparam LocalAssemblerImplementation the individual local assembler type
 * \tparam LocalAssemblerInterface the general local assembler interface
 * \tparam ExtraCtorArgs types of additional constructor arguments.
 *         Those arguments will be passed to the constructor of
 *         \c LocalAssemblerImplementation.
 *
 * The first two template parameters cannot be deduced from the arguments.
 * Therefore they always have to be provided manually.
 */
template<typename GlobalSetup,
         template <typename, typename, typename, typename, unsigned> class
         LocalAssemblerImplementation,
         typename LocalAssemblerInterface,
         typename... ExtraCtorArgs>
void createLocalAssemblers(
        const unsigned dimension,
        std::vector<MeshLib::Element*> const& mesh_elements,
        AssemblerLib::LocalToGlobalIndexMap const& dof_table,
        unsigned const integration_order,
        std::vector<std::unique_ptr<LocalAssemblerInterface>>& local_assemblers,
        ExtraCtorArgs&&... extra_ctor_args
        )
{
    DBUG("Create local assemblers.");

    switch (dimension)
    {
    case 1:
        detail::createLocalAssemblers<
            1, GlobalSetup, LocalAssemblerImplementation>(
                dof_table, mesh_elements, integration_order,
                local_assemblers,
                std::forward<ExtraCtorArgs>(extra_ctor_args)...);
        break;
    case 2:
        detail::createLocalAssemblers<
            2, GlobalSetup, LocalAssemblerImplementation>(
                dof_table, mesh_elements, integration_order,
                local_assemblers,
                std::forward<ExtraCtorArgs>(extra_ctor_args)...);
        break;
    case 3:
        detail::createLocalAssemblers<
            3, GlobalSetup, LocalAssemblerImplementation>(
                dof_table, mesh_elements, integration_order,
                local_assemblers,
                std::forward<ExtraCtorArgs>(extra_ctor_args)...);
        break;
    default:
        ERR("Meshes with dimension greater than three are not supported.");
        std::abort();
    }
}

} // ProcessLib


#endif // PROCESSLIB_UTILS_CREATE_LOCAL_ASSEMBLERS_H_