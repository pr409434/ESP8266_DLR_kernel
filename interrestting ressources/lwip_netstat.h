/********************************
http://lwip.100.n7.nabble.com/count-active-connection-on-server-side-td23014.html
Fabian Koch-2:
	http://lwip.100.n7.nabble.com/template/NamlServlet.jtp?macro=user_nodes&user=478
********************************/


unsigned long lwip_netstat(char *pcOut)

{

  char *pcStart = pcOut;

  struct lwip_sock *sock;

  struct sockaddr name;

  socklen_t namelen = sizeof(struct sockaddr_in);

  enum tcp_state eState;

  u16_t local_port, remote_port;

  ip_addr_t local_ip, remote_ip;

 

  for(int i=0, j=1; i < NUM_SOCKETS; i++){

    sock = get_socket(i);

    if(sock){

      pcOut += sprintf(pcOut, "%2u           ", j++);

      switch(sock->conn->type){

        case NETCONN_TCP:

          pcOut     += sprintf(pcOut, "TCP");

          pcOut     += sprintf(pcOut, "        ");                                            //spacer

          if(sock->conn->pcb.tcp == NULL){

            //a TCP socket that no longer has a tcp pcb. Probably in the middle of being closed

            pcOut += sprintf(pcOut, "---.---.---.---");                   //empty IP

            pcOut   += sprintf(pcOut, "   ");                                                        //spacer

            pcOut   += sprintf(pcOut, "-----");                                                      //emtpy Port

            pcOut   += sprintf(pcOut, "        ");                                     //spacer

            pcOut += sprintf(pcOut, "---.---.---.---");                   //empty IP

            pcOut   += sprintf(pcOut, "   ");                                                        //spacer

            pcOut   += sprintf(pcOut, "-----");                                                      //emtpy Port

            pcOut   += sprintf(pcOut, "          SOCKET    ");//socket API

            pcOut   += sprintf(pcOut, "???");                                                        //zombie socket

            break; //break out of the switch statement as this line is done for the socket without a pcb

          } else {

            OsIrqDisable();

            //get all states during disabled preemption so that the pcb pointer can't be nulled in between

            eState = sock->conn->pcb.tcp->state;

            local_port = sock->conn->pcb.tcp->local_port;

            local_ip = sock->conn->pcb.tcp->local_ip;

            remote_port = sock->conn->pcb.tcp->remote_port;

            remote_ip = sock->conn->pcb.tcp->remote_ip;

            OsIrqEnable();

         }

          pcOut += sprintf(pcOut, "%15s", inet_ntoa(local_ip));

          pcOut     += sprintf(pcOut, "   ");                                                               //spacer

          pcOut     += sprintf(pcOut, "%5u", local_port);

          pcOut     += sprintf(pcOut, "        ");                                            //spacer

          if(remote_ip.addr != 0){

            pcOut += sprintf(pcOut, "%15s", inet_ntoa(remote_ip));

            pcOut   += sprintf(pcOut, "   ");                                                        //spacer

            pcOut += sprintf(pcOut, "%5u", remote_port);

          } else {

            pcOut += sprintf(pcOut, "---.---.---.---");                   //empty IP

            pcOut   += sprintf(pcOut, "   ");                                                        //spacer

            pcOut   += sprintf(pcOut, "-----");                                                      //emtpy Port

          }

          pcOut     += sprintf(pcOut, "          SOCKET    ");     //socket API

          switch(eState){

            case CLOSED:

              pcOut += sprintf(pcOut, "CLOSED");

              break;

            case LISTEN:

              pcOut += sprintf(pcOut, "LISTEN");

              break;

            case ESTABLISHED:

              pcOut += sprintf(pcOut, "ESTABLISHED");

              break;

            case FIN_WAIT_1:

              pcOut += sprintf(pcOut, "FIN_WAIT_1");

              break;

            case FIN_WAIT_2:

              pcOut += sprintf(pcOut, "FIN_WAIT_2");

              break;

            case CLOSE_WAIT:

              pcOut += sprintf(pcOut, "CLOSE_WAIT");

              break;

            case TIME_WAIT:

              pcOut += sprintf(pcOut, "TIME_WAIT");

              break;

            case SYN_SENT:

            case SYN_RCVD:

              pcOut += sprintf(pcOut, "CONNECTING");

              break;

            case CLOSING:

              pcOut += sprintf(pcOut, "CLOSING");

              break;

            case LAST_ACK:

              pcOut += sprintf(pcOut, "LAST_ACK");

              break;

            default:

              pcOut += sprintf(pcOut, "???");

              break;

          }

          break;

        case NETCONN_UDP:

          pcOut     += sprintf(pcOut, "UDP");

          pcOut     += sprintf(pcOut, "        ");                                     //spacer

          pcOut += sprintf(pcOut, "%15s", sock->conn->pcb.udp->local_ip);

          pcOut     += sprintf(pcOut, "   ");                                                        //spacer

          pcOut     += sprintf(pcOut, "%5u", sock->conn->pcb.udp->local_port);

          pcOut     += sprintf(pcOut, "        ");                                     //spacer

          pcOut     += sprintf(pcOut, "               ");                 //empty foreign IP (n/a in UDP)

          pcOut     += sprintf(pcOut, "   ");                                                        //spacer

          pcOut     += sprintf(pcOut, "     ");                                                      //empty foreign port (n/a in UDP)

          pcOut     += sprintf(pcOut, "          SOCKET");         //socket API

          break;

        default:

          pcOut     += sprintf(pcOut, "???");

          break;

      }

      pcOut += sprintf(pcOut, "\r\n");

    }

  }

  return((unsigned long)(pcOut - pcStart));

}