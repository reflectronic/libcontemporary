function(cntp_define_build_dirs)
    find_path(QMAKE_PATH qmake6)
    IF(${QMAKE_PATH} STREQUAL "QMAKE_PATH-NOTFOUND")
        message(FATAL_ERROR "Couldn't call qmake. Ensure Qt 6 is installed correctly and qtpaths is located in your PATH.")
    ENDIF()

    set(QMAKE_PATH ${QMAKE_PATH}/qmake6)

    execute_process(
            COMMAND ${QMAKE_PATH} -query QT_INSTALL_PREFIX
            RESULT_VARIABLE QT_PREFIX_RESULT
            OUTPUT_VARIABLE QT_PREFIX_DIR
    )

    string(STRIP "${QT_PREFIX_DIR}" QT_PREFIX_DIR)
    file(TO_CMAKE_PATH "${QT_PREFIX_DIR}" QT_PREFIX_DIR)

    execute_process(
            COMMAND ${QMAKE_PATH} -query QT_INSTALL_PLUGINS
            RESULT_VARIABLE PLUGIN_INSTALLATION_DIR_RESULT
            OUTPUT_VARIABLE PLUGIN_INSTALLATION_DIR
    )

    string(STRIP "${PLUGIN_INSTALLATION_DIR}" PLUGIN_INSTALLATION_DIR)
    file(TO_CMAKE_PATH "${PLUGIN_INSTALLATION_DIR}" PLUGIN_INSTALLATION_DIR)

    file(RELATIVE_PATH PLUGIN_INSTALLATION_DIR "${QT_PREFIX_DIR}" "${PLUGIN_INSTALLATION_DIR}")
    set(CNTP_INSTALL_PLUGINS ${CMAKE_INSTALL_PREFIX}/${PLUGIN_INSTALLATION_DIR} PARENT_SCOPE)
endfunction()