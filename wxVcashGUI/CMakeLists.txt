project(vcash)
find_package(wxWidgets  COMPONENTS core base gl adv html xml xrc aui richtext)
if (wxWidgets_FOUND)

	include_directories(./include . ${wxWidgets_INCLUDE_DIRS})

	IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		add_definitions("-std=c++11 -D_UNICODE -D__WXOSX_COCOA__")
	else()
		add_definitions("-std=c++11 -D_UNICODE -D__WXGTK__")
	endif()

	set(SOURCE_FILES AccountPage.cpp AddressesPage.cpp BlockExplorer.cpp ConsolePage.cpp Controller.cpp
		    EntryDialog.cpp
        HistoryPage.cpp MiningPage.cpp  Resources.cpp StatisticsPage.cpp StatusBarImage.cpp
        StatusBarWallet.cpp ToolsFrame.cpp ToolsPanel.cpp TransferPage.cpp VcashApp.cpp MainFrame.cpp
        MainPanel.cpp StatusBar.cpp View.cpp OnPairsEvent.cpp WalletActions.cpp)

	add_executable(vcash ${SOURCE_FILES})
	IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		target_link_libraries(vcash coin database db_cxx db leveldb ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES}  pthread)
	else()
		target_link_libraries(vcash coin database db_cxx db leveldb ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES}  X11 pthread)
	endif()

	INSTALL(TARGETS vcash DESTINATION "/usr/local/bin")

endif()

