/**
 * All of the information (including source code, content and artwork) are copyright. No part of this message
 * or any included attachment may be reproduced, stored in a retrieval system, transmitted, broadcast or published by any means
 * (optical, magnetic, electronic, mechanical or otherwise) without the prior written permission of Market News International.
 *
 * This software is provided "AS IS," without a warranty of any kind. ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND
 * WARRANTIES, INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT,
 * ARE HEREBY EXCLUDED. "Market News International, Inc." AND ITS LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE
 * AS A RESULT OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL "Market News International" OR
 * ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL
 * OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO
 *  USE THIS SOFTWARE, EVEN IF "Market News International" HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * 
 *  Market News International, Copyright 2008
 *
 */

#include <iostream>
#include <iomanip>
#include <string>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "Constants.h"

using boost::asio::ip::tcp;

/**
 * Swap the network order of the byte array to produce a double.
 */
double swap(unsigned char * dblBuf)
{
    double swappedDblResult;
    unsigned char *dst = (unsigned char *)&swappedDblResult;
    unsigned char *src = dblBuf;

    dst[0] = src[7];
    dst[1] = src[6];
    dst[2] = src[5];
    dst[3] = src[4];
    dst[4] = src[3];
    dst[5] = src[2];
    dst[6] = src[1];
    dst[7] = src[0];

    return swappedDblResult;
}


/**
 * Example Lightning Bolt 5 client application used to demonstrate connect, login, receive and decode messages. 
 *
 * This example uses the Boost C++ Library Version 1.35.0 (see http://www.boost.org/ and http://www.boostpro.com/products/free)
 * 
 * This application has been tested on Windows XP SP2 and Red Hat Enterprise Linux 5, but should work with all platforms supported by Boost.
 * 
 */
int main(int argc, char* argv[])
{
  try
  {
    if (argc != 5)
    {
      std::cerr << "Usage: client <host> <port> <username> <password>" << std::endl;
      return 1;
    }

	std::cout << "Connecting to host:" << argv[1] << " port:" << argv[2] << std::endl;

	boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), argv[1], argv[2]);
    tcp::resolver::iterator iterator = resolver.resolve(query);
    tcp::socket socket(io_service);
    socket.connect(*iterator);
	boost::asio::socket_base::keep_alive option(true);
    socket.set_option(option);

	std::cout << "Connected successfully."<< std::endl;

	boost::system::error_code comm_error;

	// login
	std::string loginString = "AUTH ";
	loginString.append(argv[3]);
	loginString.append(" ");
	loginString.append(argv[4]);
	loginString.append("\n\n");

	std::cout << "LOGIN:" << loginString << std::endl;

	boost::asio::write(socket, boost::asio::buffer(loginString),
	  boost::asio::transfer_all(), comm_error);

	boost::array<char, 128> loginResponsebuf; 

	// wait for login response
	size_t len = socket.read_some(boost::asio::buffer(loginResponsebuf), comm_error);

	if (comm_error)
		throw boost::system::system_error(comm_error); // Some error.

	// write out login response
	std::cout.write(loginResponsebuf.data(), len);

    for (;;)
    {
	  boost::array<char, 128> msgBuf;

	  // wait for data, read header first
	  size_t header_len = boost::asio::read(socket, boost::asio::buffer(msgBuf, HEADER_SIZE));
	  std::cout << "Read " << header_len << " header bytes from socket." << std::endl;

      if (comm_error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (comm_error)
        throw boost::system::system_error(comm_error); // Some other error.

	  char *msgdata = msgBuf.c_array();
	  unsigned short int msg_length = ntohs(*(reinterpret_cast<unsigned short int *> (&msgdata[0])) );
	  std::cout << "Msg Length:" << msg_length << std::endl;
	 
	  // read payload next
	  int payload_len = boost::asio::read(socket, boost::asio::buffer(&msgBuf[HEADER_SIZE], msg_length-HEADER_SIZE));
	  std::cout << "Read " << payload_len << " payload bytes from socket." << std::endl;

	  // convert header values
   	  unsigned char* txmit_id_ptr = reinterpret_cast<unsigned char*>(&msgdata[2]);
   	  signed long int txmit_id = ((txmit_id_ptr[0]<<24)|(txmit_id_ptr[1]<<16)|(txmit_id_ptr[2]<<8)|(txmit_id_ptr[3]));

	  short type = msgdata[6];
	  short version = msgdata[7];
	  std::cout  << std::endl;
	  unsigned short int category = ntohs(*(reinterpret_cast<unsigned short int *> (&msgdata[8])) );
	  unsigned long int crc32 = ntohl(*(reinterpret_cast<unsigned long int *> (&msgdata[msg_length-CRC_SIZE])) );

      std::cout << "Message Contents:" << std::endl;
      std::cout << "unique message transmit id:" << txmit_id << " message category:" << category << " message type:" << type << " message version:"<< version << " message crc:" << crc32 << std::endl;

	  // construct an indicator from the data
	  int indicatorId = type<<24 | (version &0xff)<<16 | ( (category >> 8) &0xff)<<8 | (category &0xff);
	  std::cout << "message indicator id:" << indicatorId << std::endl;

	  std::cout << std::setprecision(6);
	  
	  int field_buffer_offset = HEADER_SIZE;	  
	  do
	  {		  
		  short field_type = msgdata[field_buffer_offset];
		  short field_id = msgdata[field_buffer_offset+1];
		  int value_offset = field_buffer_offset+2;
		  
		  switch (field_type) 
		  {
			case FLOAT_FIELD_TYPE:
				{
					long int int_bits = ntohl(*(reinterpret_cast<long int *> (&msgdata[value_offset])) );
					float field_value = *((float*)&int_bits);
					field_buffer_offset += FLOAT_INDICATOR_SIZE;
					std::cout << "field type:" << field_type << " field id:"<< field_id << " field value:" << field_value << std::endl;
				}
				break;
				
			case SHORT_FIELD_TYPE:
				{
                    unsigned char* field_value_ptr = reinterpret_cast<unsigned char*>(&msgdata[value_offset]);
				    short field_value = ((field_value_ptr[0]<<8)|(field_value_ptr[1]));
				    field_buffer_offset += SHORT_INDICATOR_SIZE;
					std::cout << "field type:" << field_type << " field id:"<< field_id << " field value:" << field_value << std::endl;
				}
				break;
				
			case LONG_FIELD_TYPE:
				{
			   	  unsigned char* field_value_ptr = reinterpret_cast<unsigned char*>(&msgdata[value_offset]);
			   	  signed long int field_value = ((field_value_ptr[0]<<24)|(field_value_ptr[1]<<16)|(field_value_ptr[2]<<8)|(field_value_ptr[3]));
			   	  field_buffer_offset += LONG_INDICATOR_SIZE;
			   	  std::cout << "field type:" << field_type << " field id:"<< field_id << " field value:" << field_value << std::endl;
				}
				break;
				
			case DOUBLE_FIELD_TYPE:
				{
					double field_value = swap((unsigned char *)&msgdata[value_offset]);
					field_buffer_offset += DOUBLE_INDICATOR_SIZE;
					std::cout << "field type:" << field_type << " field id:"<< field_id << " field value:" << field_value << std::endl;
				}
				break;
				
			case BOOL_FIELD_TYPE:
				{
					bool field_value = msgdata[value_offset];
					field_buffer_offset += BOOL_INDICATOR_SIZE;
					std::cout << "field type:" << field_type << " field id:"<< field_id << " field value:" << field_value << std::endl;
				}
				break;				
				
			case YES_NO_NA_FIELD_TYPE:				
			case DIRECTIONAL_FIELD_TYPE:
				{
					char field_value = msgdata[value_offset];	
					field_buffer_offset += DIRECTIONAL_INDICATOR_SIZE;
					std::cout << "field type:" << field_type << " field id:"<< field_id << " field value:" << (short)field_value << std::endl;
				}
				break;			

			case INT_FIELD_TYPE:
				{
					int field_value = ntohl(*(reinterpret_cast<int *> (&msgdata[value_offset])) );
					field_buffer_offset += INT_INDICATOR_SIZE;
					std::cout << "field type:" << field_type << " field id:"<< field_id << " field value:" << field_value << std::endl;
				}
				break;								
								
			default:
				{
					std::cout << "UNKNOWN DATA TYPE" << std::endl;
					field_buffer_offset = msg_length;
				}
				break;
		  } 		 
	  }
	  while(field_buffer_offset < (msg_length-CRC_SIZE));
	  
	  std::cout << std::endl;
	  
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
