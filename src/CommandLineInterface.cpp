//
// Created by kamil on 10.05.2021.
//

#include "../include/CommandLineInterface.h"

void CommandLineInterface::run() const
{
    int choice;
    int id_to_del;
    do {
        displayOpts();
        std::cin >> choice;
        std::cin.get();
        //system("clear");

        switch(choice){
            case 1:
                SessionController::getInstance().add_node(*this);
                break;
            case 2:
                std::cout << "Type node's id which you want to delete" << std::endl;
                std::cin >> id_to_del;
                std::cin.get();
                SessionController::getInstance().delete_node(id_to_del, *this);
                break;
            case 3:
                listNodes(SessionController::getInstance().getNodes());
                break;
            case 4:
                break;
            default:
                std::cout<<"Choose between 1-4"<<std::endl;
        }

        std::cin.get();
    }while(choice != 4);
}

void CommandLineInterface::displayOpts()
{
    system("clear");
    std::cout << "Session controller" << std::endl;
    std::cout << "1. Add node" << std::endl;
    std::cout << "2. Delete node" << std::endl;
    std::cout << "3. List active nodes" << std::endl;
    std::cout << "4. Quit" << std::endl;
}

void CommandLineInterface::listNodes(const std::vector<Node_info> &nodes) const
{
    std::cout<<"------------Active nodes------------"<<std::endl;
    std::cout<<std::setw(10)<<"Id"<<std::setw(10)<<"Role"<<std::endl;
    for(int i = 0; i < nodes.size(); i ++)
        std::cout << std::setw(10) << nodes.at(i).id << std::setw(10)<<nodes.at(i).role<<std::endl;
}


void CommandLineInterface::display(std::stringstream &msg) const
{
    display(msg.str());
    msg.flush();
    msg.str("");
}

void CommandLineInterface::display(const std::string &msg) const
{
    std::cout << msg << std::endl;
}
