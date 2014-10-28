#include "../include/Map/featuresMap.h"
#include <memory>
#include <stdexcept>

using namespace putslam;

/// A single instance of Elevation Map
FeaturesMap::Ptr map;

FeaturesMap::FeaturesMap(void) : Map("Features Map", MAP_FEATURES) {

}

const std::string& FeaturesMap::getName() const {
    return name;
}

/// Add new features and camera pose (initial guess) to the map
/// Position of features in relation to camera pose
void addFeatures(const std::vector<RGBDFeature>& features, const Mat34& cameraPose) {

}

/// Get all features
std::vector<MapFeature>& FeaturesMap::getAllFeatures(void) {

}

/// Get feature position
Vec3 FeaturesMap::getFeaturePosition(unsigned int id) {

}

/// get all visible features
std::vector<MapFeature>& FeaturesMap::getVisibleFeatures(const Mat34& cameraPose) {

}

/// get current pose of the sensor
Mat34 FeaturesMap::getCurrentPose(void) {

}

/// start optimization thread
void FeaturesMap::startOptimizationThread(){

}

putslam::Map* putslam::createFeaturesMap(void) {
    map.reset(new FeaturesMap());
    return map.get();
}
