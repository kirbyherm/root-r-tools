// @(#)root/core/utils:$Id: SelectionRules.cxx 41697 2011-11-01 21:03:41Z pcanal $
// Author: Velislava Spasova September 2010

/*************************************************************************
 * Copyright (C) 1995-2011, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// SelectionRules                                                       //
//                                                                      //
// the class representing all selection rules                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "SelectionRules.h"
#include <iostream>
#include "TString.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclTemplate.h"

#include "cling/Interpreter/Interpreter.h"

const clang::CXXRecordDecl *R__ScopeSearch(const char *name, const clang::Type** resultType = 0) ;

void SelectionRules::AddClassSelectionRule(ClassSelectionRule& classSel)
{
   fRulesCounter++;
   if (!classSel.HasInterpreter()) classSel.SetInterpreter(fInterp);
   if (classSel.GetIndex() < 0) classSel.SetIndex( fRulesCounter );      
   fClassSelectionRules.push_front(classSel);
}

bool SelectionRules::HasClassSelectionRules() const
{
   return !fClassSelectionRules.empty();
}

const std::list<ClassSelectionRule>& SelectionRules::GetClassSelectionRules() const
{
   return fClassSelectionRules;
}

void SelectionRules::AddFunctionSelectionRule(FunctionSelectionRule& funcSel)
{
   fRulesCounter++;
   if (!funcSel.HasInterpreter()) funcSel.SetInterpreter(fInterp);
   if (funcSel.GetIndex() < 0) funcSel.SetIndex( fRulesCounter );
   fFunctionSelectionRules.push_back(funcSel);
}

bool SelectionRules::HasFunctionSelectionRules() const
{
   return !fFunctionSelectionRules.empty();
}

const std::list<FunctionSelectionRule>& SelectionRules::GetFunctionSelectionRules() const
{
   return fFunctionSelectionRules;
}

void SelectionRules::AddVariableSelectionRule( VariableSelectionRule& varSel)
{
   fRulesCounter++;
   if (!varSel.HasInterpreter()) varSel.SetInterpreter(fInterp);
   if (varSel.GetIndex() < 0) varSel.SetIndex( fRulesCounter );
   fVariableSelectionRules.push_back(varSel);
}

bool SelectionRules::HasVariableSelectionRules() const
{
   return !fVariableSelectionRules.empty();
}

const std::list<VariableSelectionRule>& SelectionRules::GetVariableSelectionRules() const
{   
   return fVariableSelectionRules;
}

void SelectionRules::AddEnumSelectionRule(EnumSelectionRule& enumSel)
{
   fRulesCounter++;
   if (!enumSel.HasInterpreter()) enumSel.SetInterpreter(fInterp);
   if (enumSel.GetIndex() < 0) enumSel.SetIndex( fRulesCounter );   
   fEnumSelectionRules.push_back(enumSel);
}

bool SelectionRules::HasEnumSelectionRules() const
{
   return !fEnumSelectionRules.empty();
}

const std::list<EnumSelectionRule>& SelectionRules::GetEnumSelectionRules() const
{
   return fEnumSelectionRules;
}

void SelectionRules::PrintSelectionRules() const
{
   std::cout<<"Printing Selection Rules:"<<std::endl;
   if (!fClassSelectionRules.empty()) {
      int i = 0;
      for(std::list<ClassSelectionRule>::const_iterator it = fClassSelectionRules.begin(); 
          it != fClassSelectionRules.end(); ++it, ++i) {
         std::cout<<"\tClass sel rule "<<i<<":"<<std::endl;
         std::cout<< *it;
      }
   }
   else { 
      std::cout<<"\tNo Class Selection Rules"<<std::endl;
   }
   
   if (!fFunctionSelectionRules.empty()) {
      //std::cout<<""<<std::endl;
      std::list<FunctionSelectionRule>::const_iterator it2;
      int i = 0;
      
      for (it2 = fFunctionSelectionRules.begin(); it2 != fFunctionSelectionRules.end(); ++it2, ++i) {
         std::cout<<"\tFunction sel rule "<<i<<":"<<std::endl;
         std::cout<<"\t\tSelected: ";
         switch(it2->GetSelected()){
            case BaseSelectionRule::kYes: std::cout<<"Yes"<<std::endl;
               break;
            case BaseSelectionRule::kNo: std::cout<<"No"<<std::endl;
               break;
            case BaseSelectionRule::kDontCare: std::cout<<"Don't Care"<<std::endl;
               break;
            default: std::cout<<"Unspecified"<<std::endl;
         }
         it2->PrintAttributes(std::cout,2);
      }
   }
   else {
      std::cout<<"\tNo function sel rules"<<std::endl;
   }
   
   if (!fVariableSelectionRules.empty()) {
      std::list<VariableSelectionRule>::const_iterator it3;
      int i = 0;
      
      for (it3 = fVariableSelectionRules.begin(); it3 != fVariableSelectionRules.end(); ++it3, ++i) {
         std::cout<<"\tVariable sel rule "<<i<<":"<<std::endl;
         std::cout<<"\t\tSelected: ";
         switch(it3->GetSelected()){
            case BaseSelectionRule::kYes: std::cout<<"Yes"<<std::endl;
               break;
            case BaseSelectionRule::kNo: std::cout<<"No"<<std::endl;
               break;
            case BaseSelectionRule::kDontCare: std::cout<<"Don't Care"<<std::endl;
               break;
            default: std::cout<<"Unspecified"<<std::endl;
         }
         it3->PrintAttributes(std::cout,2);
      }
   }
   else {
      std::cout<<"\tNo variable sel rules"<<std::endl;
   }
   
   if (!fEnumSelectionRules.empty()) {
      std::list<EnumSelectionRule>::const_iterator it4;
      int i = 0;
      
      for (it4 = fEnumSelectionRules.begin(); it4 != fEnumSelectionRules.end(); ++it4, ++i) {
         std::cout<<"\tEnum sel rule "<<i<<":"<<std::endl;
         std::cout<<"\t\tSelected: ";
         switch(it4->GetSelected()){
            case BaseSelectionRule::kYes: std::cout<<"Yes"<<std::endl;
               break;
            case BaseSelectionRule::kNo: std::cout<<"No"<<std::endl;
               break;
            case BaseSelectionRule::kDontCare: std::cout<<"Don't Care"<<std::endl;
               break;
            default: std::cout<<"Unspecified"<<std::endl;
         }
         it4->PrintAttributes(std::cout,2);
      }
   }
   else {
      std::cout<<"\tNo enum sel rules"<<std::endl;
   }
}

void SelectionRules::ClearSelectionRules()
{
   if (!fClassSelectionRules.empty()) {
      fClassSelectionRules.clear();
   }
   if (!fFunctionSelectionRules.empty()) {
      fFunctionSelectionRules.clear();
   }
   if (!fVariableSelectionRules.empty()) {
      fVariableSelectionRules.clear();
   }
   if (!fEnumSelectionRules.empty()) {
      fEnumSelectionRules.clear();
   }
}

void SelectionRules::SetHasFileNameRule(bool file_rule)
{
   fHasFileNameRule = file_rule;
}

bool SelectionRules::GetHasFileNameRule() const
{
   if (fHasFileNameRule) return true;
   else return false;
}

void SelectionRules::SetDeep(bool deep)
{

   fIsDeep=deep;
   if (!fIsDeep) return; // the adventure stops here
   // if no selection rules, nothing to go deep into
   if (fClassSelectionRules.empty()) return;
   // Get index of the last selection rule
   long count = fClassSelectionRules.rbegin()->GetIndex() + 1;
   // Deep for classes
   // Loop on rules. If name or pattern exist, add a {pattern,name}* rule to go deep
   std::string patternString;
   for (std::list<ClassSelectionRule>::iterator classRuleIt = fClassSelectionRules.begin();
        classRuleIt != fClassSelectionRules.end(); classRuleIt++){
       if (classRuleIt->HasAttributeWithName("pattern") &&
           classRuleIt->GetAttributeValue("pattern",patternString)){
          // If the pattern already does not end with *
          if (patternString.find_last_of("*")!=patternString.size()-1){
             ClassSelectionRule csr(count++, fInterp);
             csr.SetAttributeValue("pattern", patternString+"*");
             csr.SetSelected(BaseSelectionRule::kYes);
             AddClassSelectionRule(csr);
          }
       }
       if (classRuleIt->HasAttributeWithName("name") &&
           classRuleIt->GetAttributeValue("name",patternString)){
           ClassSelectionRule csr(count++, fInterp);
           csr.SetAttributeValue("pattern", patternString+"*");
           csr.SetSelected(BaseSelectionRule::kYes);
           AddClassSelectionRule(csr);
       }
    }
//    fIsDeep = deep;
//    if (fIsDeep) {
//       long count = 0;
//       if (!fClassSelectionRules.empty()) {
//          count = fClassSelectionRules.rbegin()->GetIndex() + 1;
//       }
//       ClassSelectionRule csr(count++, fInterp);
//       csr.SetAttributeValue("pattern", "*");
//       csr.SetSelected(BaseSelectionRule::kYes);
//       AddClassSelectionRule(csr);
//       
//       ClassSelectionRule csr2(count++, fInterp);
//       csr2.SetAttributeValue("pattern", "*::*");
//       csr2.SetSelected(BaseSelectionRule::kYes);
//       AddClassSelectionRule(csr2);
//       
//       
//       // Should I disable the built-in (automatically generated) structs/classes?
//       ClassSelectionRule csr3(count++, fInterp);
//       csr3.SetAttributeValue("pattern", "__va_*"); // <built-in> 
//       csr3.SetSelected(BaseSelectionRule::kNo);
//       AddClassSelectionRule(csr3);
      //HasFileNameRule = true;
      
      //SetSelectionXMLFile(true);
//    }
}

bool SelectionRules::GetDeep() const
{
   return fIsDeep;
}

const ClassSelectionRule *SelectionRules::IsDeclSelected(clang::RecordDecl *D) const
{  
   std::string qual_name;
   GetDeclQualName(D,qual_name);
   return IsClassSelected(D, qual_name);
}

const ClassSelectionRule *SelectionRules::IsDeclSelected(clang::TypedefNameDecl *D) const
{
   std::string qual_name;
   GetDeclQualName(D,qual_name);
   return IsClassSelected(D, qual_name);
}

const ClassSelectionRule *SelectionRules::IsDeclSelected(clang::NamespaceDecl *D) const
{  
   std::string qual_name;
   GetDeclQualName(D,qual_name);
   return IsNamespaceSelected(D, qual_name);
}

const BaseSelectionRule *SelectionRules::IsDeclSelected(clang::EnumDecl *D) const
{  
   // Currently rootcling does not need any information enums.
   // Note that the code below was *not* properly matching the case
   //   typedef enum { ... } abc;
   // as the typedef is stored as an anonymous EnumDecl in clang.
   // It is likely that using a direct lookup on the name would
   // return the appropriate typedef (and then we would need to
   // select 'both' the typedef and the anonymous enums.
   return 0;
#if 0
   std::string str_name;   // name of the Decl
   std::string qual_name;  // fully qualified name of the Decl   
   GetDeclName(D, str_name, qual_name);

   if (IsParentClass(D)) {
      const BaseSelectionRule *selector = IsMemberSelected(D, str_name);
      if (!selector) // if the parent class is deselected, we could still select the enum
         return IsEnumSelected(D, qual_name);
      else           // if the parent class is selected so are all nested enums
         return selector;
   }
   
   // Enum is not part of a class
   else {
      if (IsLinkdefFile())
         return IsLinkdefEnumSelected(D, qual_name);
      return IsEnumSelected(D, qual_name);
   }
#endif
}

const BaseSelectionRule *SelectionRules::IsDeclSelected(clang::VarDecl* D) const
{  
   // Currently rootcling does not need any information about variable outside
   // of a class.
   // NOTE: In CINT the #pragma link C++ global
   // was also used to affect the dictionary for enumeration constant
   // if you reactivate this section, you may want to consider also 
   // add support for clang::EnumConstantDecl (here and in Scanner.cxx)
   return 0;
#if 0
   std::string str_name;   // name of the Decl
   std::string qual_name;  // fully qualified name of the Decl   
   GetDeclName(D, str_name, qual_name);

   if (!IsLinkdefFile())
      return IsVarSelected(D, qual_name);
   else
      return IsLinkdefVarSelected(D, qual_name);
#endif
}
 
const BaseSelectionRule *SelectionRules::IsDeclSelected(clang::FieldDecl* /* D */) const
{  
   // Currently rootcling does not need any information about fields.
   return 0;
#if 0
   std::string str_name;   // name of the Decl
   std::string qual_name;  // fully qualified name of the Decl   
   GetDeclName(D, str_name, qual_name);

   return IsMemberSelected(D, str_name);
#endif
}

const BaseSelectionRule *SelectionRules::IsDeclSelected(clang::FunctionDecl* /* D */) const
{  
   // Currently rootcling does not need any information about function.
   return 0;
#if 0
   std::string str_name;   // name of the Decl
   std::string qual_name;  // fully qualified name of the Decl   
   GetDeclName(D, str_name, qual_name);

   if (!IsLinkdefFile())
      return IsFunSelected(D, qual_name);
   else
      return IsLinkdefFunSelected(D, qual_name);
#endif
}

const BaseSelectionRule *SelectionRules::IsDeclSelected(clang::Decl *D) const
{  
   if (!D) {
      return 0;
   }
   
   clang::Decl::Kind declkind = D->getKind();
   
   switch (declkind) {
   case clang::Decl::CXXRecord: 
   case clang::Decl::ClassTemplateSpecialization:
   case clang::Decl::ClassTemplatePartialSpecialization:
      // structs, unions and classes are all CXXRecords
      return IsDeclSelected(llvm::dyn_cast<clang::RecordDecl>(D));
   case clang::Decl::Namespace:
      return IsDeclSelected(llvm::dyn_cast<clang::NamespaceDecl>(D));
   case clang::Decl::Enum:
      // Enum is part of a class
      return IsDeclSelected(llvm::dyn_cast<clang::EnumDecl>(D));
   case clang::Decl::Var:
      return IsDeclSelected(llvm::dyn_cast<clang::VarDecl>(D));
#if ROOTCLING_NEEDS_FUNCTIONS_SELECTION
   case clang::Decl::Function:
      return IsDeclSelected(llvm::dyn_cast<clang::FunctionDecl>(D));
   case clang::Decl::CXXMethod:
   case clang::Decl::CXXConstructor:
   case clang::Decl::CXXDestructor: {
      // std::string proto;
      //  if (GetFunctionPrototype(D,proto))
      //       std::cout<<"\n\tFunction prototype: "<<str_name + proto;
      //  else 
      //       std::cout<<"Error in prototype formation"; 
      std::string str_name;   // name of the Decl
      std::string qual_name;  // fully qualified name of the Decl   
      GetDeclName(D, str_name, qual_name);
      if (IsLinkdefFile()) {
         return IsLinkdefMethodSelected(D, qual_name);
      }
      return IsMemberSelected(D, str_name);
   }
#endif
   case clang::Decl::Field:
      return IsDeclSelected(llvm::dyn_cast<clang::FieldDecl>(D));
   default:
      // Humm we are not treating this case!
      return 0;
   }

   // std::string str_name;   // name of the Decl
   // std::string qual_name;  // fully qualified name of the Decl   
   // GetDeclName(D, str_name, qual_name);
   // fprintf(stderr,"IsDeclSelected: %s %s\n", str_name.c_str(), qual_name.c_str());
}


bool SelectionRules::GetDeclName(clang::Decl* D, std::string& name, std::string& qual_name) const
{
   clang::NamedDecl* N = llvm::dyn_cast<clang::NamedDecl> (D);
   
   if (N) {
      // the identifier is NULL for some special methods like constructors, destructors and operators
      if (N->getIdentifier()) { 
         name = N->getNameAsString();
      }
      else if (N->isCXXClassMember()) { // for constructors, destructors, operator=, etc. methods 
         name =  N->getNameAsString(); // we use this (unefficient) method to Get the name in that case 
      }
      llvm::raw_string_ostream stream(qual_name);
      N->getNameForDiagnostic(stream,N->getASTContext().getPrintingPolicy(),true);
      return true;
   }
   else {
      return false;
   }  
}

inline void SelectionRules::GetDeclQualName(clang::Decl* D, std::string& qual_name) const{
      clang::NamedDecl* N = static_cast<clang::NamedDecl*> (D);
      llvm::raw_string_ostream stream(qual_name);
      N->getNameForDiagnostic(stream,N->getASTContext().getPrintingPolicy(),true);
   }

bool SelectionRules::GetFunctionPrototype(clang::Decl* D, std::string& prototype) const {
   if (!D) {
      return false;
   }
   
   clang::FunctionDecl* F = llvm::dyn_cast<clang::FunctionDecl> (D); // cast the Decl to FunctionDecl
   
   if (F) {
      
      prototype = "";
      
      // iterate through all the function parameters
      for (clang::FunctionDecl::param_iterator I = F->param_begin(), E = F->param_end(); I != E; ++I) {
         clang::ParmVarDecl* P = *I;
         
         if (prototype != "")
            prototype += ",";
         
         std::string type = P->getType().getAsString();
         
         // pointers are returned in the form "int *" and I need them in the form "int*"
         if (type.at(type.length()-1) == '*') {
            type.at(type.length()-2) = '*';
            type.erase(type.length()-1);
         }
         prototype += type;
      }
      
      prototype = "(" + prototype + ")";
      // {
      //    std::string name,qual_name;
      //    GetDeclName(D,name,qual_name);
      //    std::cout << "For " << qual_name << " have prototype: '" << prototype << "'\n";
      // }
      return true;
   }
   else {
      std::cout<<"Warning - can't convert Decl to FunctionDecl"<<std::endl;
      return false;
   }
}


bool SelectionRules::IsParentClass(clang::Decl* D) const
{
   clang::DeclContext *ctx = D->getDeclContext();
   
   if (ctx->isRecord()){
      clang::Decl *parent = llvm::dyn_cast<clang::Decl> (ctx);
      if (!parent) {
         return false;
      }
      else {
         //TagDecl has methods to understand of what kind is the Decl - class, struct or union
         clang::TagDecl* T = llvm::dyn_cast<clang::TagDecl> (parent); 
         
         if (T) {
            if (T->isClass()||T->isStruct()) { 
               return true;
            }
            else {
               return false;
            }
         }
         else {
            return false;
         }
      }
   }
   else {
      return false;
   }
}


bool SelectionRules::IsParentClass(clang::Decl* D, std::string& parent_name, std::string& parent_qual_name) const
{
   clang::DeclContext *ctx = D->getDeclContext();
   
   if (ctx->isRecord()){
      clang::Decl *parent = llvm::dyn_cast<clang::Decl> (ctx);
      if (!parent) {
         return false;
      }
      else {
         //TagDecl has methods to understand of what kind is the Decl
         clang::TagDecl* T = llvm::dyn_cast<clang::TagDecl> (parent); 
         
         if (T) {
            if (T->isClass()|| T->isStruct()) { 
               GetDeclName(parent, parent_name, parent_qual_name);
               return true;
            }
            else {
               return false;
            }
         }
         else {
            return false;
         }
      }
   }
   else {
      return false;
   }
}

bool SelectionRules::GetParentName(clang::Decl* D, std::string& parent_name, std::string& parent_qual_name) const
{
   clang::DeclContext *ctx = D->getDeclContext();
   
   if (ctx->isRecord()){
      //DEBUG std::cout<<"\n\tDeclContext is Record";
      clang::Decl *parent = llvm::dyn_cast<clang::Decl> (ctx);
      if (!parent) {
         return false;
      }
      else {
         GetDeclName(parent, parent_name, parent_qual_name);
         return true;
      }
   }
   else {
      return false;
   }
}

/* This is the method that crashes
 bool SelectionRules::GetParent(clang::Decl* D, clang::Decl* parent)
 {
 clang::DeclContext *ctx = D->GetDeclContext();
 
 if (ctx->isRecord()){
 //DEBUG std::cout<<"\n\tDeclContext is Record";
 parent = llvm::dyn_cast<clang::Decl> (ctx);
 if (!parent) {
 return false;
 }
 else {
 return true;
 }
 }
 else return false;
 }
 */


// isClassSelected checks if a class is selected or not. Thre is a difference between the
// behaviour of rootcint and genreflex especially with regard to class pattern processing.
// In genreflex if we have <class pattern = "*" /> this will select all the classes 
// (and structs) found in the header file. In rootcint if we have something similar, i.e.
// #pragma link C++ class *, we will select only the outer classes - for the nested
// classes we have to specifie #pragma link C++ class *::*. And yet this is only valid
// for one level of nesting - if you need it for many levels of nesting, you will 
// probably have to implement it yourself.
// Here the idea is the following - we traverse the list of class selection rules.
// For every class we check do we have a class selection rule. We use here the
// method isSelected() (defined in BaseSelectionRule.cxx). This method returns true
// only if we have class selection rule which says "Select". Otherwise it returns 
// false - in which case we have to check wether we found a class selection rule
// which says "Veto" (noName = false and don't Care = false; OR noName = false and
// don't Care = true and we don't have neither method nor field selection rules - 
// which is for the selection.xml file case). If noName is true than we just continue - 
// this means that the current class selection rule isn't applicable for this class.

const ClassSelectionRule *SelectionRules::IsNamespaceSelected(clang::Decl* D, const std::string& qual_name) const
{
   clang::NamespaceDecl* N = llvm::dyn_cast<clang::NamespaceDecl> (D); //TagDecl has methods to understand of what kind is the Decl
   if (N==0) {
      std::cout<<"\n\tCouldn't cast Decl to NamespaceDecl";
      return 0;
   }
 
   const ClassSelectionRule *selector = 0;
   int fImplNo = 0;
   const ClassSelectionRule *explicit_selector = 0;
   const ClassSelectionRule *specific_pattern_selector = 0;
   int fFileNo = 0;
   
   // NOTE: should we separate namespaces from classes in the rules?
   std::list<ClassSelectionRule>::const_iterator it = fClassSelectionRules.begin();
   // iterate through all class selection rles
   std::string name_value;
   std::string pattern_value;
   BaseSelectionRule::EMatchType match;
   for(; it != fClassSelectionRules.end(); ++it) {
   
      match = it->Match(N,qual_name,"",IsLinkdefFile());

      if (match != BaseSelectionRule::kNoMatch) {
         // If we have a match.
         if (it->GetSelected() == BaseSelectionRule::kYes) {
            selector = &(*it);
            if (IsLinkdefFile()){               
               // rootcint prefers explicit rules over pattern rules
               if (match == BaseSelectionRule::kName) {
                  explicit_selector = &(*it);
               } else if (match == BaseSelectionRule::kPattern) {
                  // NOTE: weird ...
                  if (it->GetAttributeValue("pattern", pattern_value) && 
                      pattern_value != "*" && pattern_value != "*::*") specific_pattern_selector = &(*it);
               }
            }
         } else if (it->GetSelected() == BaseSelectionRule::kNo) {         
            if (!IsLinkdefFile()) {
               // in genreflex - we could explicitly select classes from other source files
               if (match == BaseSelectionRule::kFile) ++fFileNo; // if we have veto because of class defined in other source file -> implicit No
               else {
               
#ifdef SELECTION_DEBUG
                  std::cout<<"\tNo returned"<<std::endl;
#endif
               
                  return 0; // explicit No returned
               }
            }
            if (match == BaseSelectionRule::kPattern) {
               //this is for the Linkdef selection
               if (it->GetAttributeValue("pattern", pattern_value) && 
                   (pattern_value == "*" || pattern_value == "*::*")) ++fImplNo;
               else 
                  return 0;
            }
            else
               return 0;
         }
         else if (it->GetSelected() == BaseSelectionRule::kDontCare && !(it->HasMethodSelectionRules()) && !(it->HasFieldSelectionRules())) {
         
#ifdef SELECTION_DEBUG
            std::cout<<"Empty dontC returned = No"<<std::endl;
#endif
         
            return 0;
         }
      }
   }
   if (IsLinkdefFile()) {
      // for rootcint explicit (name) Yes is stronger than implicit (pattern) No which is stronger than implicit (pattern) Yes
      
#ifdef SELECTION_DEBUG
      std::cout<<"\n\tfYes = "<<fYes<<", fImplNo = "<<fImplNo<<std::endl;
#endif
      
      if (explicit_selector) return explicit_selector;
      else if (specific_pattern_selector) return specific_pattern_selector;
      else if (fImplNo > 0) return 0;
      else return selector;
   }
   else {                                 
      // for genreflex explicit Yes is stronger than implicit file No
      
#ifdef SELECTION_DEBUG
      std::cout<<"\n\tfYes = "<<fYes<<", fFileNo = "<<fFileNo<<std::endl;
#endif
      
      if (selector) 
         return selector;
      else 
         return 0;
   }     
   
}


const ClassSelectionRule *SelectionRules::IsClassSelected(clang::Decl* D, const std::string& qual_name) const
{
   clang::TagDecl* tagDecl = llvm::dyn_cast<clang::TagDecl> (D); //TagDecl has methods to understand of what kind is the Decl
   clang::TypedefNameDecl* typeDefNameDecl = llvm::dyn_cast<clang::TypedefNameDecl> (D);
   
   if (!tagDecl && !typeDefNameDecl) { // Ill posed
      ROOT::TMetaUtils::Error("SelectionRules::IsClassSelected",
            "Cannot cast Decl to TagDecl and Decl is not a typedef.\n");
      return 0;
      }

   if (!tagDecl && typeDefNameDecl){ // Let's try to fetch the underlying RecordDecl
      clang::RecordDecl* recordDecl = ROOT::TMetaUtils::GetUnderlyingRecordDecl(typeDefNameDecl->getUnderlyingType());
      if (!recordDecl){
         ROOT::TMetaUtils::Error("SelectionRules::IsClassSelected",
                                 "Cannot get RecordDecl behind TypedefDecl.\n");
         return 0;
      }
      tagDecl = recordDecl;
   }

   // At this point tagDecl must be well defined   
   const bool isLinkDefFile =  IsLinkdefFile();
   if (not ( isLinkDefFile || tagDecl->isClass() || tagDecl->isStruct() ))
      return 0; // Union for Genreflex
   
   const ClassSelectionRule *selector = 0;
   int fImplNo = 0;
   const ClassSelectionRule *explicit_selector = 0;
   const ClassSelectionRule *specific_pattern_selector = 0;
   int fFileNo = 0;

   // iterate through all class selection rles
   bool earlyReturn=false;
   const ClassSelectionRule* retval = nullptr;
   const clang::NamedDecl* nDecl(llvm::dyn_cast<clang::NamedDecl>(D));
   for(auto& rule : fClassSelectionRules) {
      BaseSelectionRule::EMatchType match = rule.Match(nDecl, qual_name, "", isLinkDefFile);
      if (match != BaseSelectionRule::kNoMatch) {
         // Check if the template must have its arguments manipulated                             
         if (const clang::ClassTemplateSpecializationDecl* ctsd =
         llvm::dyn_cast_or_null<clang::ClassTemplateSpecializationDecl>(D))            
            if(const clang::ClassTemplateDecl* ctd = ctsd->getSpecializedTemplate()){
               const std::string& nArgsToKeep = rule.GetAttributeNArgsToKeep();
               if (!nArgsToKeep.empty()){
                  fNormCtxt.AddTemplAndNargsToKeep(ctd->getCanonicalDecl(),
                                                   std::atoi(nArgsToKeep.c_str()));  
               }
            }
         
         if (earlyReturn) continue;
         
         // If we have a match.
         selector = &(rule);         
         if (rule.GetSelected() == BaseSelectionRule::kYes) {

            if (isLinkDefFile){
               // rootcint prefers explicit rules over pattern rules
               if (match == BaseSelectionRule::kName) {
                  explicit_selector = &(rule);
               } else if (match == BaseSelectionRule::kPattern) {
                  // NOTE: weird ...
                  const std::string& pattern_value=rule.GetAttributePattern();
                  if (!pattern_value.empty() &&
                      pattern_value != "*" &&
                      pattern_value != "*::*") specific_pattern_selector = &(rule);
               }
            }
         } else if (rule.GetSelected() == BaseSelectionRule::kNo) {

            if (!isLinkDefFile) {
               // in genreflex - we could explicitly select classes from other source files
               if (match == BaseSelectionRule::kFile) ++fFileNo; // if we have veto because of class defined in other source file -> implicit No
               else {
                  retval = selector;
                  earlyReturn=true; // explicit No returned
               }
            }
            if (match == BaseSelectionRule::kPattern) {
               //this is for the Linkdef selection
               const std::string& pattern_value=rule.GetAttributePattern();
               if (!pattern_value.empty() &&
                   (pattern_value == "*" || pattern_value == "*::*")) ++fImplNo;
               else
                  earlyReturn=true;
            }
            else
               earlyReturn=true;
         }
         else if (rule.GetSelected() == BaseSelectionRule::kDontCare && !(rule.HasMethodSelectionRules()) && !(rule.HasFieldSelectionRules())) {
            earlyReturn=true;
         }
      }
   } // Loop over the rules.
   
   if (earlyReturn) return retval;
   
   if (isLinkDefFile) {
      // for rootcint explicit (name) Yes is stronger than implicit (pattern) No which is stronger than implicit (pattern) Yes
      if (explicit_selector) return explicit_selector;
      else if (specific_pattern_selector) return specific_pattern_selector;
      else if (fImplNo > 0) return 0;
      else return selector;
   }
   else {
      // for genreflex explicit Yes is stronger than implicit file No
      return selector; // it can be nullptr
   }
   
}

const BaseSelectionRule *SelectionRules::IsVarSelected(clang::VarDecl* D, const std::string& qual_name) const
{
   std::list<VariableSelectionRule>::const_iterator it = fVariableSelectionRules.begin();
   std::list<VariableSelectionRule>::const_iterator it_end =  fVariableSelectionRules.end();
   
   const BaseSelectionRule *selector = 0;
   
   // iterate through all the rules 
   // we call this method only for genrefex variables, functions and enums - it is simpler than the class case:
   // if we have No - it is veto even if we have explicit yes as well
   for(; it != it_end; ++it) {
      if (BaseSelectionRule::kNoMatch != it->Match(llvm::dyn_cast<clang::NamedDecl>(D), qual_name, "", false)) {
         if (it->GetSelected() == BaseSelectionRule::kNo) {
            // The rule did explicitly request to not select this entity.
            return 0;
         } else {
            selector = &(*it);
         }
      }
   }
   
   return selector;
}

const BaseSelectionRule *SelectionRules::IsFunSelected(clang::FunctionDecl* D, const std::string& qual_name) const
{
   std::list<VariableSelectionRule>::const_iterator it;
   std::list<VariableSelectionRule>::const_iterator it_end;
   std::string prototype;
   
   GetFunctionPrototype(D, prototype);
   prototype = qual_name + prototype;
#ifdef SELECTION_DEBUG
   std::cout<<"\tIn isFunSelected()"<<prototype<<std::endl;
#endif
   it = fFunctionSelectionRules.begin();
   it_end = fFunctionSelectionRules.end();
      
   const BaseSelectionRule *selector = 0;
   // iterate through all the rules 
   // we call this method only for genrefex variables, functions and enums - it is simpler than the class case:
   // if we have No - it is veto even if we have explicit yes as well
   for(; it != it_end; ++it) {
      if (BaseSelectionRule::kNoMatch != it->Match(llvm::dyn_cast<clang::NamedDecl>(D), qual_name, prototype, false)) {
         if (it->GetSelected() == BaseSelectionRule::kNo) {
            // The rule did explicitly request to not select this entity.
            return 0;
         } else {
            selector = &(*it);
         }
      }
   }
   
   return selector;
}

const BaseSelectionRule *SelectionRules::IsEnumSelected(clang::EnumDecl* D, const std::string& qual_name) const
{
   std::list<VariableSelectionRule>::const_iterator it;
   std::list<VariableSelectionRule>::const_iterator it_end;
   
   it = fEnumSelectionRules.begin();
   it_end = fEnumSelectionRules.end();
   
   const BaseSelectionRule *selector = 0;
   
   // iterate through all the rules 
   // we call this method only for genrefex variables, functions and enums - it is simpler than the class case:
   // if we have No - it is veto even if we have explicit yes as well
   for(; it != it_end; ++it) {
      if (BaseSelectionRule::kNoMatch != it->Match(llvm::dyn_cast<clang::NamedDecl>(D), qual_name, "", false)) {
         if (it->GetSelected() == BaseSelectionRule::kNo) {
            // The rule did explicitly request to not select this entity.
            return 0;
         } else {
            selector = &(*it);
         }
      }
   }
   
   return selector;
}

const BaseSelectionRule *SelectionRules::IsLinkdefVarSelected(clang::VarDecl* D, const std::string& qual_name) const
{

   std::list<VariableSelectionRule>::const_iterator it;
   std::list<VariableSelectionRule>::const_iterator it_end;

   it = fVariableSelectionRules.begin();
   it_end = fVariableSelectionRules.end();
   
   const BaseSelectionRule *selector = 0;
   int fImplNo = 0;
   const BaseSelectionRule *explicit_selector = 0;
   
   std::string name_value;
   std::string pattern_value;
   for(; it != it_end; ++it) {
      BaseSelectionRule::EMatchType match 
         = it->Match(llvm::dyn_cast<clang::NamedDecl>(D), qual_name, "", false);
      if (match != BaseSelectionRule::kNoMatch) {
         if (it->GetSelected() == BaseSelectionRule::kYes) {
            // explicit rules are with stronger priority in rootcint
            if (IsLinkdefFile()){
               if (match == BaseSelectionRule::kName) {
                  explicit_selector = &(*it);
               } else if (match == BaseSelectionRule::kPattern) {
                  if (it->GetAttributeValue("pattern", pattern_value)) {
                     // NOTE: Weird ... This is a strange definition of explicit.
                     if (pattern_value != "*" && pattern_value != "*::*") explicit_selector = &(*it);
                  }
               }
            }
         }
         else {
            if (!IsLinkdefFile()) return 0;
            else {
               if (it->GetAttributeValue("pattern", pattern_value)) {
                  if (pattern_value == "*" || pattern_value == "*::*") ++fImplNo;
                  else 
                     return 0;
               }
               else
                  return 0;
            }
         }
      }
   }
   
   if (IsLinkdefFile()) {
      
#ifdef SELECTION_DEBUG
      std::cout<<"\n\tfYes = "<<fYes<<", fImplNo = "<<fImplNo<<std::endl;
#endif
      
      if (explicit_selector) return explicit_selector;
      else if (fImplNo > 0) return 0;
      else return selector;
   }
   else{
      return selector;
   }
}

const BaseSelectionRule *SelectionRules::IsLinkdefFunSelected(clang::FunctionDecl* D, const std::string& qual_name) const
{
   std::list<VariableSelectionRule>::const_iterator it;
   std::list<VariableSelectionRule>::const_iterator it_end;
   std::string prototype;
   
   GetFunctionPrototype(D, prototype);
   prototype = qual_name + prototype;
   it = fFunctionSelectionRules.begin();
   it_end = fFunctionSelectionRules.end();
   
   const BaseSelectionRule *selector = 0;
   int fImplNo = 0;
   const BaseSelectionRule *explicit_selector = 0;
   
   std::string name_value;
   std::string pattern_value;
   for(; it != it_end; ++it) {
      BaseSelectionRule::EMatchType match 
         = it->Match(llvm::dyn_cast<clang::NamedDecl>(D), qual_name, prototype, false);
      if (match != BaseSelectionRule::kNoMatch) {
         if (it->GetSelected() == BaseSelectionRule::kYes) {
            // explicit rules are with stronger priority in rootcint
            if (IsLinkdefFile()){
               if (match == BaseSelectionRule::kName) {
                  explicit_selector = &(*it);
               } else if (match == BaseSelectionRule::kPattern) {
                  if (it->GetAttributeValue("pattern", pattern_value)) {
                     // NOTE: Weird ... This is a strange definition of explicit.
                     if (pattern_value != "*" && pattern_value != "*::*") explicit_selector = &(*it);
                  }
               }
            }
         }
         else {
            if (!IsLinkdefFile()) return 0;
            else {
               if (it->GetAttributeValue("pattern", pattern_value)) {
                  if (pattern_value == "*" || pattern_value == "*::*") ++fImplNo;
                  else 
                     return 0;
               }
               else
                  return 0;
            }
         }
      }
   }
   
   if (IsLinkdefFile()) {
      
#ifdef SELECTION_DEBUG
      std::cout<<"\n\tfYes = "<<fYes<<", fImplNo = "<<fImplNo<<std::endl;
#endif
      
      if (explicit_selector) return explicit_selector;
      else if (fImplNo > 0) return 0;
      else return selector;
   }
   else{
      return selector;
   }
}

const BaseSelectionRule *SelectionRules::IsLinkdefEnumSelected(clang::EnumDecl* D, const std::string& qual_name) const
{
   std::list<VariableSelectionRule>::const_iterator it;
   std::list<VariableSelectionRule>::const_iterator it_end;

   it = fEnumSelectionRules.begin();
   it_end = fEnumSelectionRules.end();
   
   const BaseSelectionRule *selector = 0;
   int fImplNo = 0;
   const BaseSelectionRule *explicit_selector = 0;
   
   std::string name_value;
   std::string pattern_value;
   for(; it != it_end; ++it) {
      BaseSelectionRule::EMatchType match = 
         it->Match(llvm::dyn_cast<clang::NamedDecl>(D), qual_name, "", false);
      if (match != BaseSelectionRule::kNoMatch) {
         if (it->GetSelected() == BaseSelectionRule::kYes) {
            // explicit rules are with stronger priority in rootcint
            if (IsLinkdefFile()){
               if (match == BaseSelectionRule::kName){
                  explicit_selector = &(*it);
               } else if (match == BaseSelectionRule::kPattern &&
                          it->GetAttributeValue("pattern", pattern_value)) {
                  // Note: Weird ... This is a strange definition of explicit.
                  if (pattern_value != "*" && pattern_value != "*::*") explicit_selector = &(*it);
               }
            }
         }
         else {
            if (!IsLinkdefFile()) return 0;
            else {
               if (it->GetAttributeValue("pattern", pattern_value)) {
                  if (pattern_value == "*" || pattern_value == "*::*") ++fImplNo;
                  else 
                     return 0;
               }
               else
                  return 0;
            }
         }
      }
   }
   
   if (IsLinkdefFile()) {
      
#ifdef SELECTION_DEBUG
      std::cout<<"\n\tfYes = "<<fYes<<", fImplNo = "<<fImplNo<<std::endl;
#endif
      
      if (explicit_selector) return explicit_selector;
      else if (fImplNo > 0) return 0;
      else return selector;
   }
   else{
      return selector;
   }
}

// In rootcint we could select and deselect methods independantly of the class/struct/union rules
// That's why we first have to check the explicit rules for the functions - to see if there
// is rule corresponding to our method.
// Which is more - if we have (and we can have) a pattern for the parent class, than a pattern for the 
// nested class, than a pattern for certain methods in the nested class, than a rule for a 
// method (name or prototype) in the nested class - the valid rule is the last one.
// This is true irrespective of the rules (select/deselect). This is not the case for genreflex -
// in genreflex if there is a pattern deselecting something even if we have an explicit rule
// to select it, it still will not be selected.
// This method (isLinkdefMethodSelected()) might be incomplete (but I didn't have the time to think
// of anything better)
// 

const BaseSelectionRule *SelectionRules::IsLinkdefMethodSelected(clang::Decl* D, const std::string& qual_name) const
{
   std::list<FunctionSelectionRule>::const_iterator it = fFunctionSelectionRules.begin();
   std::list<FunctionSelectionRule>::const_iterator it_end = fFunctionSelectionRules.end();
   std::string prototype;
   
   GetFunctionPrototype(D, prototype);
   prototype = qual_name + prototype;
   
#ifdef SELECTION_DEBUG
   std::cout<<"\tFunction prototype = "<<prototype<<std::endl;
#endif
   
   int expl_Yes = 0, impl_r_Yes = 0, impl_rr_Yes = 0;
   int impl_r_No = 0, impl_rr_No = 0;
   const BaseSelectionRule *explicit_r = 0;
   const BaseSelectionRule *implicit_r = 0;
   const BaseSelectionRule *implicit_rr = 0;
   
   if (D->getKind() == clang::Decl::CXXMethod){
      // we first check the explicit rules for the method (in case of constructors and destructors we check the parent)
      std::string pat_value;
      for(; it != it_end; ++it) {
         BaseSelectionRule::EMatchType match
            = it->Match(llvm::dyn_cast<clang::NamedDecl>(D), qual_name, prototype, false);
         
         if (match == BaseSelectionRule::kName) {
            // here I should implement my implicit/explicit thing
            // I have included two levels of implicitness - "A::Get_*" is stronger than "*"
            explicit_r = &(*it);
            if (it->GetSelected() == BaseSelectionRule::kYes) ++expl_Yes;
            else {
                  
#ifdef SELECTION_DEBUG
                  std::cout<<"\tExplicit rule BaseSelectionRule::kNo found"<<std::endl;
#endif
                  
                  return 0; // == explicit BaseSelectionRule::kNo
                  
            }
         } else if (match == BaseSelectionRule::kPattern) {
      
            if (it->GetAttributeValue("pattern", pat_value)) {
               if (pat_value == "*") continue; // we discard the global selection rules
               
               std::string par_name, par_qual_name;
               GetParentName(D, par_name, par_qual_name);
               std::string par_pat = par_qual_name + "::*";
               
               if (pat_value == par_pat) {
                  implicit_rr = &(*it);
                  if (it->GetSelected() == BaseSelectionRule::kYes) {
                     
#ifdef SELECTION_DEBUG
                     std::cout<<"Implicit_rr rule ("<<pat_value<<"), selected = "<<selected<<std::endl;
#endif
                     
                     ++impl_rr_Yes;
                  }
                  else {
                     
#ifdef SELECTION_DEBUG
                     std::cout<<"Implicit_rr rule ("<<pat_value<<"), selected = "<<selected<<std::endl;
#endif
                     
                     ++impl_rr_No;
                  }
               }
               else {
                  implicit_r = &(*it);
                  if (it->GetSelected() == BaseSelectionRule::kYes) {
                     
#ifdef SELECTION_DEBUG
                     std::cout<<"Implicit_r rule ("<<pat_value<<"), selected = "<<selected<<std::endl;
#endif
                     
                     ++impl_r_Yes;
                  }
                  else {
                     
#ifdef SELECTION_DEBUG
                     std::cout<<"Implicit_r rule ("<<pat_value<<"), selected = "<<selected<<std::endl;
#endif
                     
                     ++impl_r_No;
                  }
               }
            }
         }
      }
   }
   if (explicit_r /*&& expl_Yes > 0*/){
      
#ifdef SELECTION_DEBUG
      std::cout<<"\tExplicit rule BaseSelectionRule::BaseSelectionRule::kYes found"<<std::endl;
#endif
      
      return explicit_r; // if we have excplicit BaseSelectionRule::kYes
   }
   else if (implicit_rr) {
      if (impl_rr_No > 0) {
         
#ifdef SELECTION_DEBUG
         std::cout<<"\tImplicit_rr rule BaseSelectionRule::kNo found"<<std::endl;
#endif
         
         return 0;
      }
      else {
         
#ifdef SELECTION_DEBUG
         std::cout<<"\tImplicit_rr rule BaseSelectionRule::kYes found"<<std::endl;
#endif
         
         return implicit_rr;
      }
   }
   else if (implicit_r) {
      if (impl_r_No > 0) {
         
#ifdef SELECTION_DEBUG
         std::cout<<"\tImplicit_r rule BaseSelectionRule::kNo found"<<std::endl;
#endif
         
         return 0;
      }
      else {
         
#ifdef SELECTION_DEBUG
         std::cout<<"\tImplicit_r rule BaseSelectionRule::kYes found"<<std::endl;
#endif
         
         return implicit_r;
      }
   }
   else {
      
#ifdef SELECTION_DEBUG
      std::cout<<"\tChecking parent class rules"<<std::endl;
#endif
      // check parent
      
      
      std::string parent_name, parent_qual_name;
      if (!GetParentName(D, parent_name, parent_qual_name)) return 0;
      
      const BaseSelectionRule *selector = 0;
      int fImplNo = 0;
      const BaseSelectionRule *explicit_selector = 0;
      
      // the same as with isClass selected
      // I wanted to use GetParentDecl and then to pass is to isClassSelected because I didn't wanted to repeat 
      // code but than GetParentDecl crashes (or returns non-sence Decl) for the built-in structs (__va_*)
      std::list<ClassSelectionRule>::const_iterator it = fClassSelectionRules.begin();
      std::string name_value;
      std::string pattern_value;
      for(; it != fClassSelectionRules.end(); ++it) {
         BaseSelectionRule::EMatchType match 
            = it->Match(llvm::dyn_cast<clang::NamedDecl>(D), parent_qual_name, "", true); // == BaseSelectionRule::kYes
         
         if (match != BaseSelectionRule::kNoMatch) {
            if (it->GetSelected() == BaseSelectionRule::kYes) {
               selector = &(*it);
          
               if (match == BaseSelectionRule::kName) {
                  explicit_selector = &(*it);
               } else if (match == BaseSelectionRule::kPattern) {
                  if (it->GetAttributeValue("pattern", pattern_value)) {
                     // NOTE: weird ...
                     if (pattern_value != "*" && pattern_value != "*::*") explicit_selector = &(*it);
                  }
               }
            }
            else { // == BaseSelectionRule::kNo
            
               if (it->GetAttributeValue("pattern", pattern_value)) {
                  if (pattern_value == "*" || pattern_value == "*::*") ++fImplNo;
                  else 
                     return 0;
               }
               else
                  return 0;
            }
         }
      }
      
#ifdef SELECTION_DEBUG
      std::cout<<"\n\tfYes = "<<fYes<<", fImplNo = "<<fImplNo<<std::endl;
#endif
      
      if (explicit_selector) {
         
#ifdef SELECTION_DEBUG
         std::cout<<"\tReturning Yes"<<std::endl;
#endif
         
         return explicit_selector;
      }
      else if (fImplNo > 0) {
#ifdef SELECTION_DEBUG
         std::cout<<"\tReturning No"<<std::endl;
#endif
         
         return 0;
      }
      else {
         
#ifdef SELECTION_DEBUG
         std::cout<<"\tReturning Yes"<<std::endl;
#endif
         
         return selector;
      }
   }
   
   return 0; 
   
}

const BaseSelectionRule *SelectionRules::IsMemberSelected(clang::Decl* D, const std::string& str_name) const
{
   std::string parent_name;
   std::string parent_qual_name;
   
   if (IsParentClass(D))
   {    
      if (!GetParentName(D, parent_name, parent_qual_name)) return 0;
      
      const BaseSelectionRule *selector = 0;
      Int_t fImplNo = 0;
      const BaseSelectionRule *explicit_selector = 0;
      int fFileNo = 0;
      
      //DEBUG std::cout<<"\n\tParent is class";
      std::list<ClassSelectionRule>::const_iterator it = fClassSelectionRules.begin();
      std::string name_value;
      std::string pattern_value;
      for(; it != fClassSelectionRules.end(); ++it) {
         BaseSelectionRule::EMatchType match 
            = it->Match(llvm::dyn_cast<clang::NamedDecl>(D), parent_qual_name, "", false);

         if (match != BaseSelectionRule::kNoMatch) {
            if (it->GetSelected() == BaseSelectionRule::kYes) {
               selector = &(*it);
               if (IsLinkdefFile()) {
                  if (match == BaseSelectionRule::kName) {
                     explicit_selector = &(*it);
                  } else if (match == BaseSelectionRule::kPattern) {
                     if (it->GetAttributeValue("pattern", pattern_value)) {
                        // NOTE: Weird ... This is a strange definition of explicit.
                        if (pattern_value != "*" && pattern_value != "*::*") explicit_selector = &(*it);
                     }
                  }
               }
            } else if (it->GetSelected() == BaseSelectionRule::kNo) {
               if (!IsLinkdefFile()) {
                  if (match == BaseSelectionRule::kFile) ++fFileNo;
                  else {
                     
#ifdef SELECTION_DEBUG
                     std::cout<<"\tNo returned"<<std::endl;
#endif
                     
                     return 0; // in genreflex we can't have that situation
                  }
               }
               else {
                  if (match == BaseSelectionRule::kPattern && it->GetAttributeValue("pattern", pattern_value)) {
                     if (pattern_value == "*" || pattern_value == "*::*") ++fImplNo;
                     else 
                        return 0;
                  }
                  else
                     return 0;
               }
            }
            else if (it->GetSelected() == BaseSelectionRule::kDontCare) // - we check the method and field selection rules for the class
            {
               if (!it->HasMethodSelectionRules() && !it->HasFieldSelectionRules()) {
                  
#ifdef SELECTION_DEBUG
                  std::cout<<"\tNo fields and methods"<<std::endl;
#endif
                  
                  return 0; // == BaseSelectionRule::kNo
               }
               else {
                  clang::Decl::Kind kind = D->getKind();
                  if (kind == clang::Decl::Field || kind == clang::Decl::CXXMethod || kind == clang::Decl::CXXConstructor || kind == clang::Decl::CXXDestructor){
                     std::list<VariableSelectionRule> members;
                     std::list<VariableSelectionRule>::iterator mem_it;
                     std::list<VariableSelectionRule>::iterator mem_it_end;
                     std::string prototype;
                     
                     if (kind == clang::Decl::Field) {
                        members = it->GetFieldSelectionRules();
                     }
                     else {
                        GetFunctionPrototype(D, prototype);
                        prototype = str_name + prototype;
                        
#ifdef SELECTION_DEBUG
                        std::cout<<"\tIn isMemberSelected (DC)"<<std::endl;
#endif
                        
                        members = it->GetMethodSelectionRules();
                     }
                     mem_it = members.begin();
                     mem_it_end = members.end();
                     for (; mem_it != mem_it_end; ++mem_it) {
                        if (BaseSelectionRule::kName == mem_it->Match(llvm::dyn_cast<clang::NamedDecl>(D), str_name, prototype, false)) {
                           if (mem_it->GetSelected() == BaseSelectionRule::kNo) return 0;
                        }
                     }
                  }
               }
            }
         }
      }
      
      if (IsLinkdefFile()) {
         
#ifdef SELECTION_DEBUG
         std::cout<<"\n\tfYes = "<<fYes<<", fImplNo = "<<fImplNo<<std::endl;
#endif
         
         if (explicit_selector) {
#ifdef SELECTION_DEBUG
            std::cout<<"\tReturning Yes"<<std::endl;
#endif
            
            return explicit_selector;
         }
         else if (fImplNo > 0) {
            
#ifdef SELECTION_DEBUG
            std::cout<<"\tReturning No"<<std::endl;
#endif
            
            return 0;
         }
         else {
            
#ifdef SELECTION_DEBUG
            std::cout<<"\tReturning Yes"<<std::endl;
#endif
            
            return selector;
         }
      }
      else {
         
         return selector;
      }
   }
   else {
      return 0;
   }
}

bool SelectionRules::IsSelectionXMLFile() const
{
   if (fSelectionFileType == kSelectionXMLFile) return true;
   else return false;
}

bool SelectionRules::IsLinkdefFile() const
{
   if (fSelectionFileType == kLinkdefFile) return true;
   else return false;
}

void SelectionRules::SetSelectionFileType(ESelectionFileTypes fileType)
{
   fSelectionFileType = fileType;
   return;
}

bool SelectionRules::AreAllSelectionRulesUsed() const {
   if (!fClassSelectionRules.empty()) {
      for(std::list<ClassSelectionRule>::const_iterator it = fClassSelectionRules.begin(); 
          it != fClassSelectionRules.end(); ++it) {
         if (BaseSelectionRule::kNo!=it->GetSelected() && !it->GetMatchFound() /* && !GetHasFileNameRule() */ ) {
            std::string name;
            if (it->GetAttributeValue("pattern", name)) {
               // keep it
            } else if (it->GetAttributeValue("name", name)) {
               // keept it
            } else {
               name.clear();
            }
            std::string file_name_value;
            if (!it->GetAttributeValue("file_name", file_name_value)) file_name_value.clear();

            if (file_name_value.length()) {
               // don't complain about defined_in rules
               continue;
            }
// For the time being a warning
//             if (IsSelectionXMLFile()) std::cout<<"Warning - ";
//             else std::cout<<"Error   - ";

            std::cout<<"Warning - ";
            if (file_name_value.length()) {
               std::cout<< "unused file name rule: \n";
               //std::cout<< file_name_value << '\n';
               it->PrintAttributes(std::cout,1);
            } else {
               std::cout<< "unused class rule: ";
               if (name.length() > 0) {
                  std::cout << name << '\n';
                  //if (name == "*") {
                  //   it->DebugPrint();
                  //}
               } else {
                  it->PrintAttributes(std::cout,1);
               }
            }
         }
      }
   }
#if Variable_rules_becomes_useful_for_rootcling
   if (!fVariableSelectionRules.empty()) {
      for(std::list<VariableSelectionRule>::const_iterator it = fVariableSelectionRules.begin(); 
          it != fVariableSelectionRules.end(); ++it) {
         if (!it->GetMatchFound() && !GetHasFileNameRule()) {
            std::string name;
            if (it->GetAttributeValue("pattern", name)) {
               // keep it
            } else if (it->GetAttributeValue("name", name)) {
               // keept it
            } else {
               name.clear();
            }            
            
            if (IsSelectionXMLFile()){
               std::cout<<"Warning - unused variable rule: "<<name<<std::endl;
            }
            else {
               std::cout<<"Error - unused variable rule: "<<name<<std::endl;
            }
            if (name.length() == 0) {
               it->PrintAttributes(std::cout,3);
            }
         }
      }
   }
#endif
#if Function_rules_becomes_useful_for_rootcling
   if (!fFunctionSelectionRules.empty()) {
      for(std::list<FunctionSelectionRule>::const_iterator it = fFunctionSelectionRules.begin(); 
          it != fFunctionSelectionRules.end(); ++it) {
         if (!it->GetMatchFound() && !GetHasFileNameRule()) {
            std::string name;
            if (it->GetAttributeValue("proto_pattern", name)) {
               // keep it
            } else if (it->GetAttributeValue("proto_name", name)) {
               // keep it
            } else if (it->GetAttributeValue("pattern", name)) {
               // keep it
            } else if (it->GetAttributeValue("name", name)) {
               // keept it
            } else {
               name.clear();
            }
            if (IsSelectionXMLFile()){
               std::cout<<"Warning - unused function rule: "<<name<<std::endl;
            }
            else {
               std::cout<<"Error - unused function rule: "<<name<<std::endl;
            }
            if (name.length() == 0) {
               it->PrintAttributes(std::cout,3);
            }
         }
      }
   }
#endif
#if Enums_rules_becomes_useful_for_rootcling
   if (!fEnumSelectionRules.empty()) {
      for(std::list<EnumSelectionRule>::const_iterator it = fEnumSelectionRules.begin(); 
          it != fEnumSelectionRules.end(); ++it) {
         if (!it->GetMatchFound() && !GetHasFileNameRule()) {
            std::string name;
            if (it->GetAttributeValue("pattern", name)) {
               // keep it
            } else if (it->GetAttributeValue("name", name)) {
               // keept it
            } else {
               name.clear();
            }            
            
            if (IsSelectionXMLFile()){
               std::cout<<"Warning - unused enum rule: "<<name<<std::endl;
            }
            else {
               std::cout<<"Error - unused enum rule: "<<name<<std::endl;
            }
            if (name.length() == 0) {
               it->PrintAttributes(std::cout,3);
            }
         }
      }
   }
#endif
   return true;
}

bool SelectionRules::SearchNames(cling::Interpreter &interp)
{
   // std::cout<<"Searching Names In Selection Rules:"<<std::endl;
   if (!fClassSelectionRules.empty()) {
      for(std::list<ClassSelectionRule>::iterator it = fClassSelectionRules.begin(),
          end = fClassSelectionRules.end();
          it != end; 
          ++it) 
      {
         if (it->HasAttributeWithName("name")) {
            std::string name_value;
            it->GetAttributeValue("name", name_value);
            // In Class selection rules, we should be interested in scopes.
            const clang::Type *typeptr = 0;
            const clang::CXXRecordDecl *target
               = ROOT::TMetaUtils::ScopeSearch(name_value.c_str(), interp,
                                               true /*diag*/, &typeptr);
            if (target) {
               it->SetCXXRecordDecl(target,typeptr);
            }
         }
      }

   }
   return true;
}


void SelectionRules::FillCache()
{
   // Fill the cache of every selection rule
   for (auto& rule : fClassSelectionRules) rule.FillCache();
   for (auto& rule : fFunctionSelectionRules) rule.FillCache();
   for (auto& rule : fVariableSelectionRules) rule.FillCache();
   for (auto& rule : fEnumSelectionRules) rule.FillCache();
}


