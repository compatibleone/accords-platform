/* ----------------------------------------------------------------------------- */
/* ACCORDS Platform                                                      	 */
/* Copyright 2012  Hamid MEDJAHED (hmedjahed@prologue.fr) Prologue               */
/* ----------------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License");               */
/* you may not use this file except in compliance with the License.              */
/* You may obtain a copy of the License at                                       */
/*                                                                               */
/*       http://www.apache.org/licenses/LICENSE-2.0                              */
/*                                                                               */
/* Unless required by applicable law or agreed to in writing, software           */
/* distributed under the License is distributed on an "AS IS" BASIS,             */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.      */
/* See the License for the specific language governing permissions and           */
/* limitations under the License.                                                */
/*-------------------------------------------------------------------------------*/

public class JcompMgr{
	public native int generateCategory(String categoryName, String categoryAttributes, String categoryActions, String categoryAccess); 
	public native int removeCategory(String categoryName);
	public native int generateComponent(String moduleName, String Categories);
	public native int removeComponent(String moduleName);
}
