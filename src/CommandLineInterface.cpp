//
// Created by kamil on 10.05.2021.
//
#include "../include/CommandLineInterface.h"

void CommandLineInterface::run()
{
    int choice;
    int selected_id;
    int selected_role;
    do {
        displayOpts();
        std::cin >> choice;
        std::cin.get();
        //system("clear");

        switch(choice){
            case 1:
                std::cout << "Select role" << std::endl;
                std::cout << "0 NONE" << std::endl;
                std::cout << "1 LEADER" << std::endl;
                std::cout << "2 VICE_LEADER" << std::endl;
                std::cin >> selected_role;
                std::cin.get();

                SessionController::getInstance().add_node(selected_role);
                break;
            case 2:
                std::cout << "Type node's id which you want to delete" << std::endl;
                std::cin >> selected_id;
                std::cin.get();
                SessionController::getInstance().delete_node(selected_id);
                break;
            case 3:
                listNodes(SessionController::getInstance().getNodes());
                break;
            case 4:
                std::cout << "Type node's id" << std::endl;
                std::cin >> selected_id;
                std::cin.get();
                displayNode(SessionController::getInstance().getNodeById(selected_id));
                break;
            case 5:
                SessionController::getInstance().cleanup();
                break;
            default:
                std::cout<<"Choose between 1-4"<<std::endl;
        }

        std::cin.get();
    }while(choice != 5);
}

void CommandLineInterface::displayOpts()
{
    system("clear");
    std::cout << "Session controller" << std::endl;
    std::cout << "1. Add node" << std::endl;
    std::cout << "2. Delete node" << std::endl;
    std::cout << "3. List active nodes" << std::endl;
    std::cout << "4. Display node role" << std::endl;
    std::cout << "5. Quit" << std::endl;
}

void CommandLineInterface::displayNode(Node_info node)
{
    std::cout<<"------------Node info------------"<<std::endl;
    std::cout << "id: " << node.id << " role: " << node.role << std::endl;
}

void CommandLineInterface::listNodes(const std::vector<Node_info> &nodes)
{
    std::cout<<"------------Active nodes------------"<<std::endl;
    std::cout<<std::setw(10)<<"Id"<<std::setw(10)<<"Role"<<std::endl;
    for(int i = 0; i < nodes.size(); i ++)
        std::cout << std::setw(10) << nodes.at(i).id << std::setw(10)<<nodes.at(i).role<<std::endl;
}


void CommandLineInterface::display(std::stringstream &msg)
{
    display(msg.str());
    msg.flush();
    msg.str("");
}

void CommandLineInterface::display(const std::string &msg)
{
    std::cout << msg << std::endl;
}
