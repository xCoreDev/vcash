project(vcash)

set(QR_VER "3.4.4")
IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(QRENCODE_ROOT_DIR "/usr/local/Cellar/qrencode/${QR_VER}/")
endif()

find_package(wxWidgets  COMPONENTS core base gl adv html xml xrc aui richtext)
if (wxWidgets_FOUND)

	include_directories(./include .
		${wxWidgets_INCLUDE_DIRS}
		${QRENCODE_ROOT_DIR}/include
		)

	IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		add_definitions("-std=c++11 -D_UNICODE -D__WXOSX_COCOA__")
	else()
		add_definitions("-std=c++11 -D_UNICODE -D__WXGTK__")
	endif()

	set(SOURCE_FILES AccountPage.cpp AddressesPage.cpp BlockExplorer.cpp ConsolePage.cpp Controller.cpp
		EntryDialog.cpp Utils.cpp wxStack.cpp ContextMenu.cpp TaskBarIcon.cpp QRDialog.cpp
		ShowInfoDialog.cpp
        HistoryPage.cpp MiningPage.cpp  Resources.cpp StatisticsPage.cpp StatusBarImage.cpp
        StatusBarWallet.cpp
				ToolsFrame.cpp
				ToolsPanel.cpp TransferPage.cpp VcashApp.cpp MainFrame.cpp
        MainPanel.cpp StatusBar.cpp View.cpp OnPairsEvent.cpp WalletActions.cpp)

	add_executable(vcash ${SOURCE_FILES})
	IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		target_link_libraries(vcash coin database db_cxx db leveldb ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES} qrencode pthread)
	else()
		target_link_libraries(vcash coin database db_cxx db leveldb ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES}  qrencode X11 pthread)
	endif()

	INSTALL(TARGETS vcash DESTINATION "/usr/local/bin")

endif()

