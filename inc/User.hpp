#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "Server.hpp"

class Server;

class User
{
public:
        User(int fd, Server *server);
        ~User();

        int getFd(void);
        std::string getNick(void);
        std::string getUsername(void);
        std::string getRealName(void);
        std::string getHostname(void);
        std::string getStatus(void);
        time_t getActivity(void);
        Server *getServer(void);
        bool getMode(const std::string &mode);
        int getChannelLimit() const;
		const std::string&	getAway() const;
        const std::vector<std::string> getChannelList() const;

        void setNick(const std::string& nick);
        void setUsername(const std::string&  username);
        void setHostname(const std::string&  hostname);
        void setRealName(const std::string&  real_name);
        void setStatus(const std::string&  status);
        void setMode(const std::string &mode, bool b);
        void setAway(const std::string&  awayMsg);
        void setChannelLimit(int aUserLimit);
        void addChannel(const std::string&  new_channel);
		void removeChannel(const std::string& name);

        void toCommand(std::string msg);
        void updateActivity(void);

        std::string message;
        std::vector<std::string> param_list;

private: // variables
        int _fd;
        std::string _nick;
        std::string _username;
        std::string _hostname;
        std::string _real_name;
        std::string _status;               // connected ou enregistre / disconnected / en attente de connexion / inactif
        std::map<std::string, bool> modes; // ex: modes["away"] = true indique que le user est away
        Server *_server;
        std::vector<std::string> _channels;
        int _channelsLimit;

        time_t _last_activity;

        std::string _command;
        std::map<std::string, void (*)(User *)> _cmd_list;

		std::string	awayMsg; // message quand le user est away

private: // method
        void execCmd(void);
        void clearCmd(void);
        /*
        inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr),remoteIP, INET6_ADDRSTRLEN)
        */
};

std::ostream &operator<<(std::ostream &o, User const &i);

#endif
