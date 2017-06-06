#include <iostream>

#include <QDir>
#include <QFileInfo>
#include <QString>

#include <AngelscriptUtils/add_on/scriptbuilder.h>

#include "CConfiguration.h"
#include "IncludeCallback.h"

int FindIncludedFile( CScriptBuilder& builder, const std::shared_ptr<const CConfiguration>& config,
					  const QString& szInclude, const QString& szFrom )
{
	if( !config )
	{
		builder.GetModule()->GetEngine()->WriteMessage(
			qPrintable( szFrom ), -1, -1, asMSGTYPE_ERROR, "No configuration file specified, cannot resolve include files" );
		return -1;
	}

	auto tryLoad = [ & ]( const QDir& dir ) -> bool
	{
		QFileInfo includedFile( dir.filePath( szInclude ) );

		if( includedFile.completeSuffix().isEmpty() )
			includedFile = dir.filePath( szInclude + config->GetFallbackExtension().c_str() );

		if( !includedFile.exists() )
			return false;

		return builder.AddSectionFromFile( includedFile.absoluteFilePath().toStdString().c_str() ) >= 0;
	};

	//First try a relative path
	if( tryLoad( QDir( QFileInfo( szFrom ).path() ) ) )
	return 1;

	std::string szPath;

	for( const auto& path : config->GetIncludePaths() )
	{
		if( tryLoad( QDir( path.c_str() ) ) )
			return 1;
	}

	QString szMessage = QString( "Couldn't find file \"%1\", included from \"%2\"" ).arg( szInclude, szFrom );

	builder.GetModule()->GetEngine()->WriteMessage(
		qPrintable( szFrom ), -1, -1, asMSGTYPE_ERROR, szMessage.toStdString().c_str() );

	return -1;
}
