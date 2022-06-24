###########################################################################
## $Id: Extensions.cmake 6460 2021-12-02 19:58:26Z nluczak $
## Authors: jezhill@gmail.com
## Description: Build information BCI2000 Framework extensions

INCLUDE_EXTENSION( DataGloveLogger          "${BCI2000_SRC_DIR}/contrib/Extensions/DataGloveLogger" )
INCLUDE_EXTENSION( WebcamLogger             "${BCI2000_SRC_DIR}/contrib/Extensions/WebcamLogger" )
INCLUDE_EXTENSION( EyetrackerLogger         "${BCI2000_SRC_DIR}/contrib/Extensions/EyetrackerLogger" )
INCLUDE_EXTENSION( EyetrackerLoggerSimulated "${BCI2000_SRC_DIR}/contrib/Extensions/EyetrackerLoggerSimulator" )
INCLUDE_EXTENSION( WiimoteLogger            "${BCI2000_SRC_DIR}/contrib/Extensions/WiimoteLogger" )
INCLUDE_EXTENSION( GazeMonitorFilter        "${BCI2000_SRC_DIR}/contrib/Extensions/GazeMonitorFilter" )
INCLUDE_EXTENSION( AudioExtension           "${BCI2000_SRC_DIR}/contrib/Extensions/AudioExtension" ) 
INCLUDE_EXTENSION( StimBoxFilter            "${BCI2000_SRC_DIR}/contrib/Extensions/StimBoxFilter" )
INCLUDE_EXTENSION( CyberGloveLogger         "${BCI2000_SRC_DIR}/contrib/Extensions/CyberGloveLogger" )
INCLUDE_EXTENSION( EyetrackerLoggerTobii3   "${BCI2000_SRC_DIR}/contrib/Extensions/EyetrackerLoggerTobii3" )
INCLUDE_EXTENSION( EyetrackerLoggerTobiiX   "${BCI2000_SRC_DIR}/contrib/Extensions/EyetrackerLoggerTobiiX" )
INCLUDE_EXTENSION( EyetrackerLoggerTobiiPro "${BCI2000_SRC_DIR}/contrib/Extensions/EyetrackerLoggerTobiiPro" )
INCLUDE_EXTENSION( EyetrackerLoggerEyeLink  "${BCI2000_SRC_DIR}/contrib/Extensions/EyetrackerLoggerEyeLink" )
INCLUDE_EXTENSION( ParallelPortFilter       "${BCI2000_SRC_DIR}/contrib/Extensions/ParallelPortFilter" )
INCLUDE_EXTENSION( XsensMTwLogger           "${BCI2000_SRC_DIR}/contrib/Extensions/XsensMTwLogger" )
INCLUDE_EXTENSION( NIDAQLogger           	"${BCI2000_SRC_DIR}/contrib/Extensions/NIDAQLogger" )
