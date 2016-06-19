2016/08/10
libvpx rebuild
----------------------------------------Change log---------------------------------------------
/ Nhuong Nguyen (2016/06/02)
// Configuration to enhance the video quality
#define CONFIG_VP9_TEMPORAL_DENOISING					0
#define CONFIG_SPATIAL_SVC						1
#define CONFIG_TEMPORAL_DENOISING					1
#define CONFIG_DEPENDENCY_TRACKING					1
#define CONFIG_EMULATE_HARDWARE						1
#define CONFIG_ERROR_CONCEALMENT					1

#define CONFIG_SPATIAL_RESAMPLING					1
#define CONFIG_COEFFICIENT_RANGE_CHECKING				1
#define CONFIG_MULTI_RES_ENCODING					1

#define CONFIG_POSTPROC							1
#define CONFIG_POSTPROC_VISUALIZER					1
------------------------------------------------------------------------------------------------
