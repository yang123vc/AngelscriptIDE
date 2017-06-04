add_sources(
	CAbout.cpp
	CAbout.h
	CAngelscriptSyntaxHighlighter.cpp
	CAngelscriptSyntaxHighlighter.h
	CASMainWindow.cpp
	CASMainWindow.h
	CBaseOutputWidget.cpp
	CBaseOutputWidget.h
	CCodeTextEdit.cpp
	CCodeTextEdit.h
	CCompileOutputWidget.cpp
	CCompileOutputWidget.h
	CFindDialog.cpp
	CFindDialog.h
	CInformationOutputWidget.cpp
	CInformationOutputWidget.h
	CLineNumberArea.cpp
	CLineNumberArea.h
	COutputWidget.cpp
	COutputWidget.h
	CScriptCodeTextEdit.cpp
	CScriptCodeTextEdit.h
	CUI.cpp
	CUI.h
)

add_forms(
	CAbout.ui
	CASMainWindow.ui
	CFindDialog.ui
	COutputWidget.ui
)

add_subdirectory( options )
add_subdirectory( util )
