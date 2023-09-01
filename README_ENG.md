# Entity Component System

**Duration:** 2023/03/01 → 2023/08/13
**Skills:** C++, CMake, ECS

**Github [ECS Project](https://github.com/Winteradio/ECS-with-Scene-and-Node)**

**Github [Log Project](https://github.com/Winteradio/Log)**

**Github [Memory Project](https://github.com/Winteradio/MemoryPool)**

---

## Summary

- Rebuilding ECS (Entity Component System).

## 🛠️ Technology Stack and Libraries

- **Language**
    - C++
- **Libraries**
    - Log Project
    - Memory Project
    - Rpc (UUID)

## 📝 Concept Explanation

### What is ECS (Entity Component System)?

- A DOTS-based system under development in Unity.
- **DOTS vs OOP**
    - **What is DOTS?**
        - Data-Oriented Technology Stack.
        - A data-centric programming approach.
    - **How is it different from OOP?**
        - Data-centric
            - Focuses on optimizing performance and accessing data directly.
        - Continuous memory accessibility
            - Unlike OOP objects, stores data of the same type in contiguous memory.
            - Optimizes memory access patterns to increase cache hit probability.
        - Ease of parallel processing
            - Allows independent construction of methods (or systems) for different data types.
            - Systems operate on data, not objects (entities), enabling separation of objects and systems.
- **Entity Component System (ECS)**
    - **What is an Entity?**
        - An abstraction representing individual objects such as game objects or characters.
        - Serves as a simple identifier (naming) and does not directly hold data.
        - Example:
            - Entity ID: "John Doe"
                - "Abilities" Component - "John Doe"
                - "Skills" Component - "John Doe"
                - "Weapon" Component - "John Doe"
            - In this example, the Entity serves as an identifier for the components.
    - **What is a Component?**
        - Represents actual data used by entities.
        - Identified by Entity ID when attached to an entity.
        - Components of the same type should be stored in contiguous memory.
    - **What is a System?**
        - Logic that processes and manipulates components.
        - Each system has access rights to the components it requires.
        - Systems can be processed in parallel, but dependencies between them must be considered.
        - Example:
            - System: "Render"
                - Uses "Render Component"
            - System: "Move"
                - Uses "Move Component"

### Types of ECS: Sparse Set vs. Archetype

- **Sparse Set**
    - A method of linking Entity IDs with each component.
    - Uses a Dense Set to store Entity IDs and a Sparse Set for components.
    - Allows rapid access to components in contiguous memory using the Entity ID.
    - Related Site: [Entt Library on Github](https://github.com/skypjack/entt)

- **Archetype**
    - Groups of entities with similar component compositions.
    - Each archetype has a unique component composition.
    - Unlike Sparse Set, it doesn't store components in contiguous memory but stores similar types of archetypes in contiguous memory.
    - Related Site: [Unity's ECS Archetype](https://docs.unity3d.com/Packages/com.unity.entities@1.0/manual/concepts-archetypes.html)

### Why Implement ECS in C++, an OOP Language?

- **Understanding Game Engine Structure**
    - Need for a continuously usable library.
    - Lack of understanding of how the engine's internals work.
        - How project files are structured.
        - How play, pause, and building within the engine work.
        - Whether the engine and the game are developed separately.
    - Difficulty in implementing information from UML diagrams of other engines.

- **Implementation of Independent Systems**
    - Need for a basic engine to implement functionality like collision and fluid simulation.

- **Different Approach to ECS**
    - Issues with Sparse Set:
        - Limited to accessing only one component via the Entity ID.
        - Not all systems can use just one component.
    - Issues with Archetype:
        - Even if you want to add just one or two components to a specific archetype, you need to create a new archetype by duplicating data.
        - Example:
            - When you want to create objects that emit light and render, you need a new "Light Render Archetype."
            - This would require duplicating the required components and removing them from the existing archetype.
    - **Introduction of the "Node" Concept**
        - Utilize a concept similar to nodes often seen in Unity and Unreal Engine.
        - Each node has access rights to specific components, combining concepts from Sparse Set and Archetype.
        - Example:
            - When creating an object that emits light and renders, you can register both a "Render Node" and a "Light Node" for the same entity.
            - This allows the same Transform Component to be used for both.
            - Systems that transform data can be restructured or controlled to prevent conflicts if they exist.

## 📝 Development Details

### Interfaces

- **IObject**
    - Interface for objects using MyUUID.
        
        ```cpp
        class IObject
        {
        public:
            IObject() {};
            IObject(MyUUID ID) : m_ID(ID) {};
            virtual ~IObject() {};
        
        public:
            void SetID(MyUUID ID) { m_ID = ID; }
            MyUUID GetID() { return m_ID; }
            MyUUID GetID() const { return m_ID; }
        
        protected:
            MyUUID m_ID;
        };
        ```

- **IComponent**
    - Interface for objects that ComponentManager stores as IComponent pointers.
        
        ```cpp
        class IComponent : public IObject
        {
        public:
            IComponent() {};
            IComponent(MyUUID ID) : IObject(ID) {};
            virtual ~IComponent() {};
        };
        ```

- **INode**
    - Interface for objects that NodeManager stores as INode pointers.
        - **Check(MemoryPtr<Entity>& EntityMPtr)**
            - Checks whether the Entity has the required components via NodeManager.
        - **Init(MemoryPtr<Entity>& EntityMPtr)**
            - Acquires MemoryPtr to the required components via ComponentManager and Entity if the Entity has them.
            
        ```cpp
        class INode : public IObject
        {
        public:
            INode() {};
            INode(MyUUID ID) : IObject(ID) {}
            virtual ~INode() {};
        
        public:
            virtual bool Check(MemoryPtr<Entity>& EntityMPtr) = 0;
            virtual void Init(MemoryPtr<Entity>& EntityMPtr) = 0;
        };
        ```
    - Each Node has the same ID as the Entity.
        - This is because there can be no duplicate nodes for the same Entity.
        
- **ISystem**
    - Interface for objects that SystemManager stores as ISystem pointers.
    - **m_NodeType**
        - Information about the Node type required by the system.
        - Used by Scene's Update function to provide MyUUID Set data based on the required Node types.
    - **Update(float DeltaTime, int SceneIndex, MyUUIDUnSet& NodeIDData) = 0;**
        - **SceneIndex**
            - Required to process systems within the context of the scene.
        
        ```cpp
        class ISystem : public IObject
        {
        public:
            ISystem() {};
            ISystem(MyUUID ID) : IObject(ID) {};
            virtual ~ISystem() {};
        
        public:
            virtual void Update(float DeltaTime, int SceneIndex, MyUUIDUnSet& NodeIDData) = 0;
        
            virtual void SetNodeType(const std::type_info* Type) = 0;
            const std::type_info*& GetNodeType() { return m_NodeType; }
        
        protected:
            const std::type_info* m_NodeType;
        };
        ```

## Managers

- **SceneManager**
  - **Main Role**
    - Responsible for creating and deleting scenes.
  - **Scene-Related**
    - Creates scenes
      - Assigns an index to scenes
      - Since scenes are not created frequently, UUIDs are unnecessary.
    - Accesses scenes using an index.

- **EntityManager**
  - **Main Role**
    - Responsible for creating and deleting entities.
  - **Entity-Related**
    - Creates entities
    - Accesses entities using IDs.

- **ComponentManager**
  - **Main Role**
    - Responsible for creating and deleting components.
  - **Component-Related**
    - Creates components
    - Accesses components using IDs and type information.
    - Retrieves complete type information.
      - Retrieves all ID information for a given type.
        - For file saving and loading and for UI usage.

- **SystemManager**
  - **Main Role**
    - Responsible for creating and deleting systems.
  - **System-Related**
    - Creates systems
    - Accesses systems using IDs.
    - Retrieves all ID information.
    - **Allows for setting dependencies.**
      - **SetDependency(MyUUID& MainID, MyUUID& DependentID)**
        - **MainID**
          - ID of the system that is being depended upon.
        - **DependentID**
          - ID of the system that depends on another.
        - Checks if both IDs exist in the SystemManager.
          - If not, no action is taken.
        - Adds the DependencyIDData for the MainID.
          - A collection of IDs that depend on the MainID.
        - Creates a TestIDSet for testing cycle dependencies.
        - Checks for cycles in the dependency graph using a topological sort.
          - If a cycle is detected, the dependency is deleted.

          ```cpp
          void SystemManager::SetDependency(MyUUID& MainID, MyUUID& DependencyID)
          {
              bool Check = HasSystem(MainID) && HasSystem(DependencyID);
              if (!Check) return;
  
              m_DependencyIDData[MainID].insert(DependencyID);
  
              MyUUIDUnSet TestIDSet;
              for (auto [ID, System] : m_ISystemMPtrData)
              {
                  TestIDSet.insert(ID);
              }
  
              Check = TopologySort(TestIDSet);
              if (!Check) { DeleteDependency(MainID, DependencyID); }
          }
          ```

      - **DeleteDependency(MyUUID& MainID, MyUUID& DependentID)**
        - **MainID**
          - ID of the system that is being depended upon.
        - **DependentID**
          - ID of the system that depends on another.
        - Checks if MainID exists in the SystemManager.
          - If not, no action is taken.
        - Deletes the dependency of DependentID on MainID.

          ```cpp
          void SystemManager::DeleteDependency(MyUUID& MainID, MyUUID& DependentID)
          {
              bool Check = HasSystem(MainID) && HasSystem(DependentID);
              if (Check)
              {
                  auto ITR = m_DependencyIDData[MainID].find(DependentID);
                  if (ITR != m_DependencyIDData[MainID].end())
                  {
                      Log::Info("Erase Dependency %s for %s", typeid(*GetSystem(MainID)).name(), typeid(*GetSystem(DependentID)).name());
                      m_DependencyIDData[MainID].erase(ITR);
                  }
              }
          }
          ```

  - **Cycle Graph Detection**
    - **TopologySort(MyUUIDUnSet& SystemIDData)**
      - Implements Kahn's algorithm for topological sorting.
      - **TopoQueue**
        - Queue containing valid IDs with an indegree of 0.
      - **TopoIndegreeData**
        - Stores indegree information for each ID.
      - First assigns an index to IDs with an indegree of 0 and pushes them to TopoQueue.
      - Iteratively removes IDs from TopoQueue, marks them as visited, and decrements indegrees of dependent IDs.
      - Continues until TopoQueue is empty.
      - If the number of assigned indices matches the total number of SystemIDData, the graph is acyclic.
      - Otherwise, it indicates the presence of cycles.

        ```cpp
        bool SystemManager::TopologySort(MyUUIDUnSet& SystemIDData)
        {
            MyUUIDQueue TopoQueue;
            DependentIndegreeUnMap TopoIndegreeData = CalculateIndegree(m_DependencyIDData);
  
            int Index = 0;
            for (auto& [ID, Indegree] : TopoIndegreeData)
            {
                if (Indegree == 0)
                {
                    m_SystemIndexData[ID] = ++Index;
  
                    TopoQueue.push(ID);
                    Indegree--;
                }
            }
  
            while (!TopoQueue.empty())
            {
                MyUUID& ID = TopoQueue.front();
                for (auto& DependencyID : m_DependencyIDData[ID])
                {
                    TopoIndegreeData[DependencyID]--;
  
                    if (TopoIndegreeData[DependencyID] == 0)
                    {
                        m_SystemIndexData[DependencyID] = ++Index;
                        TopoQueue.push(DependencyID);
                        TopoIndegreeData[DependencyID]--;
                    }
                }
  
                TopoQueue.pop();
            }
  
            if (Index == SystemIDData.size()) return true;
            else
            {
                Log::Warn("This Graph is cyclic");
                return false;
            }
        }
        ```

    - **CalculateIndegree(DependencyIDUnSetUnMap& DependencyIDData)**
      - **DependencyIDData**
        - Contains DependencyID (dependee) information with DependentID (dependent) data.
      - **DependentIndegreeData**
        - Stores the indegree count for each DependentID.

        ```cpp
        using DependencyIDUnSetUnMap = MyUUIDUnMap<MyUUIDUnSet>;
        using DependentIndegreeUnMap = MyUUIDUnMap<int>;
  
        SystemManager::DependentIndegreeUnMap SystemManager::CalculateIndegree(DependencyIDUnSetUnMap& DependencyIDData)
        {
            DependentIndegreeUnMap DependentIndegreeData;
  
            for (auto& [ID, IDData] : DependencyIDData) { DependentIndegreeData[ID] = 0; }
  
            for (auto& [ID, IDData] : DependencyIDData)
            {
                for (auto& DependencyID : IDData)
                {
                    DependentIndegreeData[DependencyID]++;
                }
            }
  
            return DependentIndegreeData;
        }
        ```
  
- **NodeManager**
  - **Main Role**
    - Responsible for creating and deleting nodes.
  - **Node-Related**
    - Accesses nodes using IDs.
    - Creates nodes through entities.
      - Creates nodes using the provided entity.
      - Checks the required components for the node.
        - If the check fails, the node is deleted.
        - If the check passes, data is recorded in the NodeData.
      - **template <typename T> MemoryPtr<T> Create(MemoryPtr<Entity>& Object)**

        ```cpp
        template <typename T>
        MemoryPtr<T> NodeManager::Create(MemoryPtr<Entity>& Object)
        {
            bool Check = HasMPtrMap<T>();
            if (!Check) CreateMPtrMap<T>();
  
            MemoryPtr<T> NodeMPtr = MemoryManager::GetHandle().Create<T>(Object->GetID());
  
            Check = NodeMPtr->Check(Object);
  
            if (!Check)
            {
                MemoryManager::GetHandle().Delete<T>(NodeMPtr);
                throw Except("NodeManager | %s | This Entity has not components for this node", __FUNCTION__, typeid(T).name());
            }
  
            NodeMPtr->Init(Object);
            GetIDData<T>().insert(NodeMPtr->GetID());
            GetMPtrMapData<T>()[NodeMPtr->GetID()] = NodeMPtr;
  
            return NodeMPtr;
        }
        ```

## 💻 ECS Example Code

### Setting Up Cyclic Systems

- Code
    
    ```cpp
    void Example()
    {
        MemoryManager::GetHandle().Init();
    
        // Initializing the System Manager
        SystemManager::GetHandle().Init();
    
        // Creating each System using the System Manager
        MemoryPtr<ISystem> Render = SystemManager::GetHandle().Create<RenderSystem>();
        MemoryPtr<ISystem> Move = SystemManager::GetHandle().Create<MoveSystem>();
        MemoryPtr<ISystem> Collision = SystemManager::GetHandle().Create<CollisionSystem>();
        MemoryPtr<ISystem> Physics = SystemManager::GetHandle().Create<PhysicsSystem>();
        MemoryPtr<ISystem> Camera = SystemManager::GetHandle().Create<CameraSystem>();
    
        // Setting dependencies for each system
        SystemManager::GetHandle().SetDependency( Physics->GetID(), Collision->GetID() );
        SystemManager::GetHandle().SetDependency( Collision->GetID(), Move->GetID() );
        SystemManager::GetHandle().SetDependency( Move->GetID(), Render->GetID() );
        SystemManager::GetHandle().SetDependency( Camera->GetID(), Render->GetID() );
    
        // This dependency relation between RenderSystem and PhysicsSystem creates a cyclic graph
        // Therefore, this relation is deleted by the system manager
        SystemManager::GetHandle().SetDependency( Render->GetID(), Physics->GetID() ); 
    }
    ```
    
- Result
    
    ```bash
    ...
    **01:55:13:478 | WARN  This Graph is cyclic
    01:55:13:478 | INFO  Erase Dependency class RenderSystem for class PhysicsSystem**
    ...
    ```
    
    - Dependent System - Systems they depend on
        - Physics System - Collision System
        - Collision System - Move System
        - Move System - Render System
        - Camera System - Render System
        - Render System -
            - In the current state, a non-cyclic graph is maintained
        - Physics System - Collision System, **Render System**
        - Collision System - Move System
        - Move System - Render System
        - Camera System - Render System
        - Render System -
            - When Physics System depends on Render System, 
            a cyclic graph is created
                - **01:55:13:478 | WARN  This Graph is cyclic**
            - The dependency of Physics System on Render System is removed by the system manager
                - **01:55:13:478 | INFO  Erase Dependency class RenderSystem for class PhysicsSystem**

### Creating a Render Node but Missing Required Components

- Code
    
    ```cpp
    void Example()
    {
        MemoryManager::GetHandle().Init();
    
        // Initializing System Manager
        EntityManager::GetHandle().Init();
        NodeManager::GetHandle().Init();
        ComponentManager::GetHandle().Init();
    
        // Creating an Entity and Registering it in the Main Scene
        MemoryPtr<Entity> Object = EntityManager::GetHandle().Create();
    
        // Creating a Component
        MemoryPtr<MeshComponent> Mesh = ComponentManager::GetHandle().Create<MeshComponent>();
    
        // Adding Mesh Component and Transform Component to the Object Entity
        Object->AddComponent<MeshComponent>( Mesh->GetID() );
    
        // Creating a RenderNode
        MemoryPtr<RenderNode> Node = NodeManager::GetHandle().Create<RenderNode>( Object );
    
        EntityManager::GetHandle().Destroy();
        NodeManager::GetHandle().Destroy();
        ComponentManager::GetHandle().Destroy();
        MemoryManager::GetHandle().Destroy();
    }
    
    // RenderNode class
    class RenderNode : public INode
    {
        public :
            RenderNode() {};
            RenderNode( MyUUID ID ) : INode( ID ) {};
            virtual ~RenderNode(){};
    
        public :
            virtual bool Check( MemoryPtr<Entity>& Object )
            {
                if ( !Object->HasComponent<MeshComponent>() ) return false;
                if ( !Object->HasComponent<TransformComponent>() ) return false;
    
                return true;
            }
    
            virtual void Init( MemoryPtr<Entity>& Object )
            {
                if ( !Check( Object ) ) return;
    
                Mesh = ComponentManager::GetHandle().GetComponent<MeshComponent>( Object->GetComponent<MeshComponent>() );
                Trans = ComponentManager::GetHandle().GetComponent<TransformComponent>( Object->GetComponent<TransformComponent>() );
            }
    
        public :
            MemoryPtr<MeshComponent> Mesh;
            MemoryPtr<TransformComponent> Trans;
    };
    ```
    
- Result
    
    ```bash
    ...
    **02:03:01:812 | INFO  Instance | class RenderNode | 0000028478690C20 | Create new
    02:03:01:812 | INFO  Instance | class RenderNode | 0000028478690C20 | Delete
    02:03:01:812 | ERROR  NodeManager | NodeManager::Create | This Entity has not components for this node**
    ...
    ```
    
    - A Render Node requires both Mesh Component and Transform Component
    - However, the Entity in question has only registered the Mesh Component

### Waiting for the Creation of a Physics Node

- Code
    
    ```cpp
    void Example()
    {
    		// ... //
    
    		// Creating each System using the System Manager
        MemoryPtr<ISystem> Render = SystemManager::GetHandle().Create<RenderSystem>();
        MemoryPtr<ISystem> Physics = SystemManager::GetHandle().Create<PhysicsSystem>();
    
    		// Creating a Scene
        MemoryPtr<Scene> Main = SceneManager::GetHandle().Create();
    
    		// Creating an Entity and Registering it in the Main Scene
        MemoryPtr<Entity> Object = EntityManager::GetHandle().Create();
    
        // ... //
    
        // Creating a RenderNode through the Object Entity
        MemoryPtr<RenderNode> Node_1 = NodeManager::GetHandle().Create<RenderNode>( Object );
        Main->RegisterNode<RenderNode>( Node_1->GetID() );
    
        // Registering some systems in the Main Scene
        Main->RegisterSystem( Render->GetID() );
        Main->RegisterSystem( Physics->GetID() );
    
        // Frame in the Scene
        Main->Update( 0.0f );
    
        // Creating a PhysicsNode through the Object Entity
        MemoryPtr<PhysicsNode> Node_2 = NodeManager::GetHandle().Create<PhysicsNode>( Object );
        Main->RegisterNode<PhysicsNode>( Node_2->GetID() );
    
        // Frame in the Scene
        Main->Update( 0.0f );
    
    	  // ... //
    }
    ```
    
- Result
    
    ```bash
    ...
    **02:15:43:771 | INFO  RenderSystem Update**
    02:15:43:771 | WARN  MemoryManager | class PhysicsNode | There isn't existed this type of MemoryPool
    02:15:43:771 | INFO  MemoryManager | class PhysicsNode | Create new pool ptr list
    02:15:43:771 | INFO  MemoryManager | class PhysicsNode | Pool Ptr 000002735C2E1AC0 - Start Ptr 000002735C2E7A30 | Create new memory pool
    02:15:43:771 | INFO  Instance | class PhysicsNode | 000002735C2E7A30 | Create new
    **02:15:43:772 | INFO  PhysicsSystem Update
    02:15:43:772 | INFO  RenderSystem Update**
    ...
    ```
    
    - The Main Scene has Physics System and Render System registered.
    - However, there are no nodes available for the Physics System.
    - As a result, the update proceeds from the Main as only the Render System is valid.
    - When a Physics Node is added to the Main Scene, the Physics System becomes valid, and the update proceeds.

## 💡 Learning Experiences

### Understanding the Internal Structure of Game Engines

- While I haven't yet built a complete game engine from scratch, I feel like I have gained a fundamental understanding of how the internal components should be structured.
- In the past, when working on code, I didn't have a clear sense of how to add features or classes, but with this project, I have a direction for designing better solutions when adding systems, nodes, components, or improving functionalities within managers.
- Unlike my previous projects, where I simply added source files to the project and used them, I aimed to create libraries just like in a real engine. I created static and dynamic libraries, and in the future, using libraries with ECS or a game engine that uses ECS should be more convenient with proper configuration using CMake.

### Reflections on Objects

- Creating multiple interfaces during this project led me to revisit the concepts of objects and inheritance in object-oriented programming (OOP).
- Originally, for NodeManager and ComponentManager, I used void* or std::any to store MemoryPtr for different types, but I wanted to explore a different approach.
- I created interfaces, INode and IComponent, corresponding to Node and Component, and implemented success or failure of creation based on the inheritance relationship between these interfaces and the current objects, using dynamic_cast. During this process, I gained a deeper understanding of RTTI (Run-Time Type Information) and vtables.
- While overusing RTTI can introduce overhead and affect performance, I believe it's not a significant concern at my current level of development.

### Importance of Algorithms

- Implementing topological sorting in the System Manager for testing dependencies was an enlightening experience. Prior to this, I simply placed systems in linked lists and allowed users to set dependencies manually.
- Initially, I thought that systems were limited to RenderNode, MoveNode, and CollisionNode, so I felt it would be manageable.
- However, I discovered that topological sorting is used more extensively in games than I initially thought. For instance, it's used in quests with dependencies or checking dependencies for specific character classes, among other scenarios. I realized that if I couldn't implement even the basic dependencies between systems correctly, it would be a problem.
- After implementing dependency checks using a DAG (Directed Acyclic Graph) and topological sorting for system sequencing, it became clear that this approach was far more convenient than manually creating linked lists. I also grappled with questions like whether to check dependencies once all systems are registered or after each dependency is set.
- When checking dependencies after all systems are registered, if a cyclic graph emerges, it's unclear which system to remove. Therefore, I chose to check dependencies for every system as they are registered and remove dependencies if a cycle is detected.