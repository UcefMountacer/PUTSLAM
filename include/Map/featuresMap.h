/** @file featuresMap.h
 *
 * implementation - Elevation Map
 *
 */

#ifndef FEATURES_MAP_H_INCLUDED
#define FEATURES_MAP_H_INCLUDED

#include "map.h"
#include "../PoseGraph/graph_g2o.h"
#include <iostream>
#include <memory>
#include <atomic>
#include "../include/Grabber/depthSensorModel.h"

#define FATURES_START_ID 10000

namespace putslam {
/// create a single Map
Map* createFeaturesMap(void);
/// create a single Map - overloaded
Map* createFeaturesMap(std::string configFileGrabber, std::string sensorConfig);
}

using namespace putslam;

class MapModifier{
    /// Features to update
    std::vector<MapFeature> features2update;

    /// Features to remove
    std::vector<int> removeIds;

    /// Features to update
    std::vector<MapFeature> features2add;

    /// Update features?
    inline bool updateFeatures() { return (features2update.size()>0) ?  true : false;};
    /// Remove feaures?
    inline bool removeFeatures() { return (removeIds.size()>0) ?  true : false;};
    /// add features?
    inline bool addFeatures() { return (features2add.size()>0) ?  true : false;};

    /// mutex to lock access
    std::recursive_mutex mtxMapVisualization;
};

/// Map implementation
class FeaturesMap: public Map {
public:
	/// Pointer
	typedef std::unique_ptr<FeaturesMap> Ptr;

	/// Construction
	FeaturesMap(void);

	/// Construction
	FeaturesMap(std::string configFileGrabber, std::string sensorConfig);

	/// Destruction
	~FeaturesMap(void);

	/// Name of the map
	const std::string& getName() const;

	/// Add NEW features and a NEW camera pose (initial guess) to the map
	/// Position of features in relation to camera pose, default: the last sensor pose
	void addFeatures(const std::vector<RGBDFeature>& features,
			int poseId = -1);

	/// add measurements (features measured from the last camera pose) default: the last sensor pose
	void addMeasurements(const std::vector<MapFeature>& features,
			int poseId = -1);

	/// add new pose of the camera, returns id of the new pose
	int addNewPose(const Mat34& cameraPose, float_type timestamp);

	/// Get all features
	std::vector<MapFeature> getAllFeatures(void);

	/// Get feature position
	Vec3 getFeaturePosition(unsigned int id);

	/// get all visible features
	std::vector<MapFeature> getVisibleFeatures(const Mat34& cameraPose);

	/// get pose of the sensor (default: last pose)
	Mat34 getSensorPose(int poseId = -1);

	/// start optimization thread
	void startOptimizationThread(unsigned int iterNo, int verbose = 0);

	/// Wait for optimization thread to finish
	void finishOptimization(std::string trajectoryFilename,
			std::string graphFilename);

private:
	///camera trajectory
	std::vector<Mat34> camTrajectory;

	///Pose graph
	Graph * poseGraph;

	/// Depth sensor model
	DepthSensorModel sensorModel;

	/// Optimization thread
	std::unique_ptr<std::thread> optimizationThr;

	/// optimization flag
	std::atomic<bool> continueOpt;

	/// Number of features
	unsigned int featureIdNo;

	/// boolean value informing if the features had been added to the map
	bool emptyMap;

    ///Set of features (map for the front-end thread)
    std::vector<MapFeature> featuresMapFrontend;

    /// mutex for critical section - map frontend
    std::recursive_mutex mtxMapFrontend;

    ///Set of features (map for the visualization thread)
    std::vector<MapFeature> featuresMapVisualization;

    /// mutex for critical section - map visualization
    std::recursive_mutex mtxMapVisualization;

    ///Set of features (map for the map management thread)
    std::vector<MapFeature> featuresMapManagement;

    /// mutex for critical section - map management
    std::recursive_mutex mtxMapManagemet;

    /// Map frontend -- buffer
    MapModifier bufferMapFrontend;

    /// Map visualization -- buffer
    MapModifier bufferMapVisualization;

    /// Map management -- buffer
    MapModifier bufferMapmanagement;

	/// optimization thread
	void optimize(unsigned int iterNo, int verbose);
};

#endif // FEATURES_MAP_H_INCLUDED
