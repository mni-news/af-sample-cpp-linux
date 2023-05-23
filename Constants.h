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

#pragma once

static const int HEADER_SIZE               = 10; // Size of header on the wire
static const int CRC_SIZE                  = 4; // Size of trailing CRC on the wire

static const short FLOAT_FIELD_TYPE        = 0; // Id of float on the wire
static const short FLOAT_FIELD_SIZE        = 4; // Size of float on the wire
static const short SHORT_FIELD_TYPE        = 2; // Id of short on the wire
static const short SHORT_FIELD_SIZE        = 2; // Size of short on the wire
static const short LONG_FIELD_TYPE         = 3; // Id of long on the wire
static const short LONG_FIELD_SIZE         = 8; // Size of long on the wire
static const short DOUBLE_FIELD_TYPE       = 4; // Id of double on the wire
static const short DOUBLE_FIELD_SIZE       = 8; // Size of double on the wire
static const short BOOL_FIELD_TYPE         = 6; // Id of boolean on the wire
static const short BOOL_FIELD_SIZE         = 1; // Size of boolean on the wire
static const short YES_NO_NA_FIELD_TYPE    = 7; // Id of yes_no_na on the wire
static const short YES_NO_NA_FIELD_SIZE    = 1; // Size of yes_no_na on the wire
static const short DIRECTIONAL_FIELD_TYPE  = 8; // Id of directional on the wire
static const short DIRECTIONAL_FIELD_SIZE  = 1; // Size of directional on the wire
static const short INT_FIELD_TYPE          = 9; // Id of int on the wire
static const short INT_FIELD_SIZE          = 4; // Size of int on the wire

static const short FLOAT_INDICATOR_SIZE       = FLOAT_FIELD_SIZE + 2; // Size of type/id/float on the wire
static const short SHORT_INDICATOR_SIZE       = SHORT_FIELD_SIZE + 2; // Size of type/id/short on the wire	  
static const short LONG_INDICATOR_SIZE        = LONG_FIELD_SIZE + 2; // Size of type/id/long on the wire	  
static const short DOUBLE_INDICATOR_SIZE      = DOUBLE_FIELD_SIZE + 2; // Size of type/id/double on the wire	  
static const short BOOL_INDICATOR_SIZE        = BOOL_FIELD_SIZE + 2; // Size of type/id/boolean on the wire	  
static const short YES_NO_NA_INDICATOR_SIZE   = YES_NO_NA_FIELD_SIZE + 2; // Size of type/id/yes_no_na on the wire	  
static const short DIRECTIONAL_INDICATOR_SIZE = DIRECTIONAL_FIELD_SIZE + 2; // Size of type/id/directional on the wire	  	  
static const short INT_INDICATOR_SIZE         = INT_FIELD_SIZE + 2; // Size of type/id/int on the wire	  
