PROJECT( COMMS )

EXECUTE_PROCESS(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/6_4_0/gsoap_build.bat" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/6_4_0"
    OUTPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/6_4_0/gsoap_build.out"
    ERROR_FILE "${CMAKE_CURRENT_SOURCE_DIR}/6_4_0/gsoap_build.err"
)

SET ( SRCS
    Account.cpp
    Account.h
    #Active.cpp
    #Active.h
    #Analysis.cpp
    #Analysis.h
    #attrHelper.cpp
    #attrHelper.h
    Attribute.cpp
    Attribute.h
    AttributeImpl.h
    AttributeHistory.cpp
    AttributeHistory.h
    AttributeType.cpp
    AttributeType.h
    Autoupdate.cpp
    Autoupdate.h
    Cluster.cpp
    Cluster.h
    comms.cpp
    comms.h
    Dali.cpp
    Dali.h
    Depository.h
    dfu.cpp
    dfu.h
    dfuFile.cpp
    dfuFile.h
    dfuServer.cpp
    dfuServer.h
    #DfuWorkunit.cpp
    #DfuWorkunit.h
    DiskAttribute.h
    DiskAttribute.cpp
    DiskAttributeHistory.cpp
    DiskModule.h
    DiskModule.cpp
    DiskMonitor.h
    DiskMonitor.cpp
    DiskRepository.cpp
    DiskRepository.h
    DropZone.cpp
    DropZone.h
    EclCC.cpp
    EclCC.h
    EclMeta.cpp
    EclMeta.h
    EclServer.cpp
    EclServer.h
    #Entity.cpp
    #Entity.h
    #EntityInstance.cpp
    #EntityInstance.h
    #FileSpray.cpp
    #FileSpray.h
    #FileSprayFile.cpp
    #FileSprayFile.h
    #FileSprayPath.cpp
    #FileSprayPath.h
    Graph.cpp
    Graph.h
    Group.cpp
    Group.h
    GSoapUtil.cpp
    GSoapUtil.h
    #HtmlHelper.cpp
    #HtmlHelper.h
    LocalFile.cpp
    LocalFile.h
    LocalResult.cpp
    LocalResult.h
    LocalWorkunit.cpp
    LocalWorkunit.h
    machine.h
    meta.h
    Migration.cpp
    Migration.h
    ModFileAttribute.cpp
    ModFileAttributeHistory.cpp
    ModFileModule.cpp
    ModFileRepository.cpp
    ModFileRepository.h
    Module.cpp
    Module.h
    ModuleHelper.cpp
    ModuleHelper.h
    Repository.cpp
    Repository.h
    RepositoryImpl.h
    RequestQueue.h
    resource.h
    Result.cpp
    Result.h
    ResultRequestQueue.cpp
    ResultRequestQueue.h
    SecureSoapSocketClient.cpp
    SecureSoapSocketClient.h
    SMC.cpp
    SMC.h
    SMCCluster.cpp
    SMCCluster.h
    SMCVersion.cpp
    SMCVersion.h
    Topology.cpp
    Topology.h
    WebService.cpp
    #WebPage.cpp
    #WebPage.h
    WebService.h
    Workspace.cpp
    Workspace.h
    WorkspaceItem.cpp
    WorkspaceItem.h
    Workunit.cpp
    Workunit.h
    #WsAccount.h
    #WsAnalysis.h
    #WsAttributes.h
    #WsDfu.h
    #WsEclNet.h
    #WsEclNetHelper.cpp
    #WsEclNetHelper.h
    #WsECLWorkunits.h
    #WsEntityClassify.h
    #WsEntityEnhance.h
    #WsEnvironment.h
    #WsFileSpray.h
    #WsRepository.h
    #WsSEEP.h
    #WsSMC.h
    #WsTopology.h
    #WsUpdate.h
    #WsUpdateHelper.cpp
    #WsUpdateHelper.h
    #WsUsers.h
    #WsWorkunits.h
    #WsWorkunitsEx.h
    XGMMLWorkunit.cpp
    XGMMLWorkunit.h
)

INCLUDE_DIRECTORIES (
    ../clib
    ../ecllib

    ${WTL_INCLUDE_DIR}/include
    ${Boost_INCLUDE_DIRS}

    ${GSOAP_DIR}
    ${GSOAP_DIR}/extras
    ${GSOAP_DIR}/mod_gsoap/gsoap_win/wininet
)

ADD_DEFINITIONS( -DWITH_SOAPDEFS_H )

ADD_MSVC_PRECOMPILED_HEADER("stdafx.h" "stdafx.cpp" SRCS)

ADD_LIBRARY ( COMMS SHARED 
    ${SRCS}
    comms.rc

    ${GSOAP_DIR}/stdsoap2.cpp
    ${GSOAP_DIR}/mod_gsoap/gsoap_win/wininet/gsoapWinInet.cpp
    ${GSOAP_DIR}/mod_gsoap/gsoap_win/wininet/gsoapWinInet.h
)

ADD_DEPENDENCIES ( COMMS
    CLIB
)

TARGET_LINK_LIBRARIES( COMMS
    CLIB
    ECLLIB
    msxml2.lib
    comsupp.lib
    authz.lib
    ${Boost_LIBRARIES}
)

INSTALL ( TARGETS COMMS RUNTIME DESTINATION bin )
