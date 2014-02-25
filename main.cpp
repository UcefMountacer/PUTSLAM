#include <iostream>
#include <thread>
#include "include/Defs/putslam_defs.h"
#include "Grabber/kinect_grabber.h"
#include "PoseGraph/graph_g2o.h"
#include "PoseGraph/global_graph.h"
#include "Tracker/trackerKLT.h"
#include "3rdParty/tinyXML/tinyxml2.h"
#include <cmath>
#include <ctime>
#include <ratio>
#include <chrono>

using namespace std;

std::unique_ptr<std::thread> thread_poseGraph;
std::unique_ptr<std::thread> thread_globalGraph;

void globalGraphUpdate(Graph* global_graph, const VertexSE3& transform){
    global_graph->addVertexPose(transform); //update graph
    global_graph->optimize(10); // loop closure detection

}

void poseGraphUpdate(Graph* graph, Graph* global_graph, const VertexSE3& transform){
    if(graph->addVertexPose(transform)){ //detect previously visited places and update graph (add vertex or node to previously visited vertex)
        if (thread_globalGraph) {
            thread_globalGraph->join(); //wait until global graph thread is comleted (it should be considered as an error)
            thread_globalGraph.release(); //release object (is it possible to start thread without 'new'?)
        }
        thread_globalGraph = std::unique_ptr<std::thread> (new std::thread(&globalGraphUpdate, global_graph, transform)); // throw thread
        graph->optimize(10);
    }
}

unsigned const max_tracking_duration = 6;//seconds

int main()
{
    try {
        using namespace putslam;
        auto begin = std::chrono::high_resolution_clock::now();
        auto tt = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::microseconds> (tt - begin).count();
auto tt1 = std::chrono::high_resolution_clock::now();
std::cout << std::chrono::duration_cast<std::chrono::microseconds> (tt1 - begin).count();
//now_c = std::chrono::system_clock::to_time_t(std::chrono::high_resolution_clock::now());
//std::cout << std::setprecision (numeric_limits<double>::digits10 + 1) << std::chrono::high_resolution_clock::now().count() << std::endl;
        tinyxml2::XMLDocument config;
        config.LoadFile("configGlobal.xml");
        if (config.ErrorID())
            std::cout << "unable to load config file.\n";
        std::string grabberType(config.FirstChildElement( "Grabber" )->FirstChildElement( "name" )->GetText());

        Grabber* grabber;
        if (grabberType == "Kinect")
            grabber = createGrabberKinect();
        else if (grabberType == "MesaImaging")
            grabber = createGrabberKinect();
        else // Default
            grabber = createGrabberKinect();

        // create objects and print configuration
        cout << "Current grabber: " << grabber->getName() << std::endl;
        int framerate = 0;
        config.FirstChildElement( "Grabber" )->FirstChildElement( "framerate" )->QueryIntText(&framerate);
        cout << "Grabber framerate: " << framerate << std::endl;
        Tracker * tracker = createTrackerKLT();
        cout << "Current tracker: " << tracker->getName() << std::endl;
        Graph * graph = createPoseGraphG2O();
        cout << "Current graph: " << graph->getName() << std::endl;
        Graph * global_graph = createGlobalGraph();
        cout << "Current global graph: " << global_graph->getName() << std::endl;

        auto start = chrono::system_clock::now();
        while (1){ //tracking
            grabber->grab(); // grab frame
            if (!tracker->track(grabber->getSensorFrame())) { //check if tracker should start new tracking
                if (thread_poseGraph) {
                    thread_poseGraph->join(); //wait until pose graph thread is comleted (it should be considered as an error)
                    thread_poseGraph.release(); //release object (is it possible to start thread without 'new'?)
                }
                thread_poseGraph = unique_ptr<thread>(new thread(&poseGraphUpdate, graph, global_graph, tracker->getVertex())); // throw thread
                tracker->reset();
            }
            if (chrono::duration_cast<chrono::duration<unsigned> >(chrono::system_clock::now() - start).count()>max_tracking_duration){
                thread_poseGraph->join();
                thread_globalGraph->join();
                break;
            }
        }

    }
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
