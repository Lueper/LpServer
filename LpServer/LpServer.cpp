// 비동기 서버

#include "LpServer.h"

using namespace boost;

LpServer::LpServer() {
    m_acceptor = new lpnet::LpAcceptor();
}

LpServer::LpServer(const std::string _ip, uint16_t _port) {
    m_acceptor = new lpnet::LpAcceptor();
    m_acceptor->Bind(_ip, _port);
}

LpServer::~LpServer() {
    delete m_acceptor;
}

void LpServer::LoadFile(std::string _filePath) {
    try {
#ifdef _DEBUG
        YAML::Node config = YAML::LoadFile(_filePath)["Debug"];
#else
        YAML::Node config = YAML::LoadFile(_filePath)["Release"];
#endif
        SetThreadCount(config["Server"]["ThreadCount"].as<uint32_t>());
        SetIOBufferSize(config["Server"]["IOBufferSize"].as<uint32_t>());

        std::cout << "[Info]#YAML : Load Config file is Success" << "\n";
    }
    catch (const YAML::BadFile& e) {
        std::cerr << e.msg << std::endl;
    }
    catch (const YAML::ParserException& e) {
        std::cerr << e.msg << std::endl;
    }
}

void LpServer::Start() {
    // 클라이언트 연결 대기
    m_acceptor->Listen();

    // 비동기 승인 시작
    m_acceptor->AsyncAccept();

    // 이벤트가 없을 때까지 대기
    for (uint32_t i = 0; i < m_threadCount; i++) {
        m_ThreadVector.emplace_back(std::thread {
                std::bind(&LpServer::Run, this)
        });
    }

    for (auto& thread : m_ThreadVector) {
        thread.join();
    }
}

void LpServer::Run() {
    lpnet::LpIOContext::Instance().Run();
}