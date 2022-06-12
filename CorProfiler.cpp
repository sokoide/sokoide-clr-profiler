// Copyright (c) .NET Foundation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#include "CorProfiler.h"
#include "CComPtr.h"
#include "corhlpr.h"
#include "profiler_pal.h"
#include <string>

const IID IID_ICorProfilerInfo8 = {
    0xC5AC80A6,
    0x782E,
    0x4716,
    {0x80, 0x44, 0x39, 0x59, 0x8C, 0x60, 0xCF, 0xBF}};
const IID IID_ICorProfilerCallback = {
    0x176FBED1,
    0xA55C,
    0x4796,
    {0x98, 0xCA, 0xA9, 0xDA, 0x0E, 0xF8, 0x83, 0xE7}};
const IID IID_ICorProfilerCallback2 = {
    0x8A8CC829,
    0xCCF2,
    0x49fe,
    {0xBB, 0xAE, 0x0F, 0x02, 0x22, 0x28, 0x07, 0x1A}};
const IID IID_ICorProfilerCallback3 = {
    0x4FD2ED52,
    0x7731,
    0x4b8d,
    {0x94, 0x69, 0x03, 0xD2, 0xCC, 0x30, 0x86, 0xC5}};
const IID IID_ICorProfilerCallback4 = {
    0x7B63B2E3,
    0x107D,
    0x4d48,
    {0xB2, 0xF6, 0xF6, 0x1E, 0x22, 0x94, 0x70, 0xD2}};
const IID IID_ICorProfilerCallback5 = {
    0x8DFBA405,
    0x8C9F,
    0x45F8,
    {0xBF, 0xFA, 0x83, 0xB1, 0x4C, 0xEF, 0x78, 0xB5}};
const IID IID_ICorProfilerCallback6 = {
    0xFC13DF4B,
    0x4448,
    0x4F4F,
    {0x95, 0x0C, 0xBA, 0x8D, 0x19, 0xD0, 0x0C, 0x36}};
const IID IID_ICorProfilerCallback7 = {
    0xF76A2DBA,
    0x1D52,
    0x4539,
    {0x86, 0x6C, 0x2A, 0xA5, 0x18, 0xF9, 0xEF, 0xC3}};
const IID IID_ICorProfilerCallback8 = {
    0x5BED9B15,
    0xC079,
    0x4D47,
    {0xBF, 0xE2, 0x21, 0x5A, 0x14, 0x0C, 0x07, 0xE0}};

PROFILER_STUB EnterStub(FunctionIDOrClientID functionId,
                        COR_PRF_ELT_INFO eltInfo) {
    printf("\r\nEnter %" UINT_PTR_FORMAT "", (UINT64)functionId.functionID);
}

PROFILER_STUB LeaveStub(FunctionID functionId, COR_PRF_ELT_INFO eltInfo) {
    printf("\r\nLeave %" UINT_PTR_FORMAT "", (UINT64)functionId);
}

PROFILER_STUB TailcallStub(FunctionID functionId, COR_PRF_ELT_INFO eltInfo) {
    printf("\r\nTailcall %" UINT_PTR_FORMAT "", (UINT64)functionId);
}

EXTERN_C void EnterNaked(FunctionIDOrClientID functionIDOrClientID,
                         COR_PRF_ELT_INFO eltInfo);
EXTERN_C void LeaveNaked(FunctionIDOrClientID functionIDOrClientID,
                         COR_PRF_ELT_INFO eltInfo);
EXTERN_C void TailcallNaked(FunctionIDOrClientID functionIDOrClientID,
                            COR_PRF_ELT_INFO eltInfo);

CorProfiler::CorProfiler() : refCount(0), corProfilerInfo(nullptr) {}

CorProfiler::~CorProfiler() {
    if (this->corProfilerInfo != nullptr) {
        this->corProfilerInfo->Release();
        this->corProfilerInfo = nullptr;
    }
}

HRESULT STDMETHODCALLTYPE
CorProfiler::Initialize(IUnknown* pICorProfilerInfoUnk) {
    printf("CorProfiler::Initialize is called\n");

    HRESULT queryInterfaceResult = pICorProfilerInfoUnk->QueryInterface(
        __uuidof(ICorProfilerInfo8),
        reinterpret_cast<void**>(&this->corProfilerInfo));

    if (FAILED(queryInterfaceResult)) {
        return E_FAIL;
    }

    // set event mask
    DWORD eventMask =
        COR_PRF_MONITOR_ENTERLEAVE | COR_PRF_ENABLE_FUNCTION_ARGS |
        COR_PRF_ENABLE_FUNCTION_RETVAL | COR_PRF_ENABLE_FRAME_INFO;

    auto hr = this->corProfilerInfo->SetEventMask(eventMask);
    if (hr != S_OK) {
        printf("ERROR: Profiler SetEventMask failed (HRESULT: %d)", hr);
    }

    // set function enter/leave callback
    // TODO
#ifdef ENTER_LEAVE_SUPPORTED
    hr = this->corProfilerInfo->SetEnterLeaveFunctionHooks3WithInfo(
        EnterNaked, LeaveNaked, TailcallNaked);

    if (hr != S_OK) {
        printf("ERROR: Profiler SetEnterLeaveFunctionHooks3WithInfo failed "
               "(HRESULT: %d)",
               hr);
    }
#endif

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::Shutdown() {
    printf("CorProfiler::Shutdown is called\n");
    if (this->corProfilerInfo != nullptr) {
        this->corProfilerInfo->Release();
        this->corProfilerInfo = nullptr;
    }

    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::AppDomainCreationStarted(AppDomainID appDomainId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::AppDomainCreationFinished(
    AppDomainID appDomainId, HRESULT hrStatus) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::AppDomainShutdownStarted(AppDomainID appDomainId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::AppDomainShutdownFinished(
    AppDomainID appDomainId, HRESULT hrStatus) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::AssemblyLoadStarted(AssemblyID assemblyId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::AssemblyLoadFinished(AssemblyID assemblyId, HRESULT hrStatus) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::AssemblyUnloadStarted(AssemblyID assemblyId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::AssemblyUnloadFinished(AssemblyID assemblyId, HRESULT hrStatus) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ModuleLoadStarted(ModuleID moduleId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ModuleLoadFinished(ModuleID moduleId,
                                                          HRESULT hrStatus) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ModuleUnloadStarted(ModuleID moduleId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ModuleUnloadFinished(ModuleID moduleId,
                                                            HRESULT hrStatus) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ModuleAttachedToAssembly(
    ModuleID moduleId, AssemblyID AssemblyId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ClassLoadStarted(ClassID classId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ClassLoadFinished(ClassID classId,
                                                         HRESULT hrStatus) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ClassUnloadStarted(ClassID classId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ClassUnloadFinished(ClassID classId,
                                                           HRESULT hrStatus) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::FunctionUnloadStarted(FunctionID functionId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::JITCompilationStarted(FunctionID functionId, BOOL fIsSafeToBlock) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::JITCompilationFinished(
    FunctionID functionId, HRESULT hrStatus, BOOL fIsSafeToBlock) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::JITCachedFunctionSearchStarted(
    FunctionID functionId, BOOL* pbUseCachedFunction) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::JITCachedFunctionSearchFinished(
    FunctionID functionId, COR_PRF_JIT_CACHE result) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::JITFunctionPitched(FunctionID functionId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::JITInlining(FunctionID callerId,
                                                   FunctionID calleeId,
                                                   BOOL* pfShouldInline) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ThreadCreated(ThreadID threadId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ThreadDestroyed(ThreadID threadId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ThreadAssignedToOSThread(
    ThreadID managedThreadId, DWORD osThreadId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RemotingClientInvocationStarted() {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::RemotingClientSendingMessage(GUID* pCookie, BOOL fIsAsync) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::RemotingClientReceivingReply(GUID* pCookie, BOOL fIsAsync) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RemotingClientInvocationFinished() {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::RemotingServerReceivingMessage(GUID* pCookie, BOOL fIsAsync) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RemotingServerInvocationStarted() {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RemotingServerInvocationReturned() {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::RemotingServerSendingReply(GUID* pCookie, BOOL fIsAsync) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::UnmanagedToManagedTransition(
    FunctionID functionId, COR_PRF_TRANSITION_REASON reason) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ManagedToUnmanagedTransition(
    FunctionID functionId, COR_PRF_TRANSITION_REASON reason) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON suspendReason) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RuntimeSuspendFinished() { return S_OK; }

HRESULT STDMETHODCALLTYPE CorProfiler::RuntimeSuspendAborted() { return S_OK; }

HRESULT STDMETHODCALLTYPE CorProfiler::RuntimeResumeStarted() { return S_OK; }

HRESULT STDMETHODCALLTYPE CorProfiler::RuntimeResumeFinished() { return S_OK; }

HRESULT STDMETHODCALLTYPE
CorProfiler::RuntimeThreadSuspended(ThreadID threadId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RuntimeThreadResumed(ThreadID threadId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::MovedReferences(
    ULONG cMovedObjectIDRanges, ObjectID oldObjectIDRangeStart[],
    ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[]) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ObjectAllocated(ObjectID objectId,
                                                       ClassID classId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ObjectsAllocatedByClass(
    ULONG cClassCount, ClassID classIds[], ULONG cObjects[]) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::ObjectReferences(ObjectID objectId, ClassID classId,
                              ULONG cObjectRefs, ObjectID objectRefIds[]) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RootReferences(ULONG cRootRefs,
                                                      ObjectID rootRefIds[]) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::ExceptionThrown(ObjectID thrownObjectId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::ExceptionSearchFunctionEnter(FunctionID functionId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionSearchFunctionLeave() {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::ExceptionSearchFilterEnter(FunctionID functionId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionSearchFilterLeave() {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::ExceptionSearchCatcherFound(FunctionID functionId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::ExceptionOSHandlerEnter(UINT_PTR __unused) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::ExceptionOSHandlerLeave(UINT_PTR __unused) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::ExceptionUnwindFunctionEnter(FunctionID functionId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionUnwindFunctionLeave() {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::ExceptionUnwindFinallyEnter(FunctionID functionId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionUnwindFinallyLeave() {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::ExceptionCatcherEnter(FunctionID functionId, ObjectID objectId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionCatcherLeave() { return S_OK; }

HRESULT STDMETHODCALLTYPE CorProfiler::COMClassicVTableCreated(
    ClassID wrappedClassId, REFGUID implementedIID, void* pVTable,
    ULONG cSlots) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::COMClassicVTableDestroyed(
    ClassID wrappedClassId, REFGUID implementedIID, void* pVTable) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionCLRCatcherFound() {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ExceptionCLRCatcherExecute() {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ThreadNameChanged(ThreadID threadId,
                                                         ULONG cchName,
                                                         WCHAR name[]) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::GarbageCollectionStarted(
    int cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::SurvivingReferences(
    ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[],
    ULONG cObjectIDRangeLength[]) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::GarbageCollectionFinished() {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::FinalizeableObjectQueued(DWORD finalizerFlags, ObjectID objectID) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::RootReferences2(
    ULONG cRootRefs, ObjectID rootRefIds[], COR_PRF_GC_ROOT_KIND rootKinds[],
    COR_PRF_GC_ROOT_FLAGS rootFlags[], UINT_PTR rootIds[]) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::HandleCreated(GCHandleID handleId,
                                                     ObjectID initialObjectId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::HandleDestroyed(GCHandleID handleId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::InitializeForAttach(
    IUnknown* pCorProfilerInfoUnk, void* pvClientData, UINT cbClientData) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ProfilerAttachComplete() { return S_OK; }

HRESULT STDMETHODCALLTYPE CorProfiler::ProfilerDetachSucceeded() {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ReJITCompilationStarted(
    FunctionID functionId, ReJITID rejitId, BOOL fIsSafeToBlock) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::GetReJITParameters(ModuleID moduleId, mdMethodDef methodId,
                                ICorProfilerFunctionControl* pFunctionControl) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::ReJITCompilationFinished(FunctionID functionId, ReJITID rejitId,
                                      HRESULT hrStatus, BOOL fIsSafeToBlock) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ReJITError(ModuleID moduleId,
                                                  mdMethodDef methodId,
                                                  FunctionID functionId,
                                                  HRESULT hrStatus) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::MovedReferences2(
    ULONG cMovedObjectIDRanges, ObjectID oldObjectIDRangeStart[],
    ObjectID newObjectIDRangeStart[], SIZE_T cObjectIDRangeLength[]) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::SurvivingReferences2(
    ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[],
    SIZE_T cObjectIDRangeLength[]) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::ConditionalWeakTableElementReferences(
    ULONG cRootRefs, ObjectID keyRefIds[], ObjectID valueRefIds[],
    GCHandleID rootIds[]) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::GetAssemblyReferences(
    const WCHAR* wszAssemblyPath,
    ICorProfilerAssemblyReferenceProvider* pAsmRefProvider) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CorProfiler::ModuleInMemorySymbolsUpdated(ModuleID moduleId) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::DynamicMethodJITCompilationStarted(
    FunctionID functionId, BOOL fIsSafeToBlock, LPCBYTE ilHeader,
    ULONG cbILHeader) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CorProfiler::DynamicMethodJITCompilationFinished(
    FunctionID functionId, HRESULT hrStatus, BOOL fIsSafeToBlock) {
    return S_OK;
}
