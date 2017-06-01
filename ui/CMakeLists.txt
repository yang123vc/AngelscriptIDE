add_sources(
	CASMainWindow.cpp
	CASMainWindow.h
	CLineNumberArea.cpp
	CLineNumberArea.h
	CCodeTextEdit.cpp
	CCodeTextEdit.h
	CScriptCodeTextEdit.cpp
	CScriptCodeTextEdit.h
	CAbout.cpp
	CAbout.h
	COutputWidget.cpp
	COutputWidget.h
	CBaseOutputWidget.cpp
	CBaseOutputWidget.h
	CCompileOutputWidget.cpp
	CCompileOutputWidget.h
	CInformationOutputWidget.cpp
	CInformationOutputWidget.h
	CUI.cpp
	CUI.h
	CFindDialog.cpp
	CFindDialog.h
)

add_forms(
	CAbout.ui
	CASMainWindow.ui
	COutputWidget.ui
	CFindDialog.ui
)

add_subdirectory( options )
add_subdirectory( util )
