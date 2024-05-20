import WrongNetworkMessage from '../components/WrongNetworkMessage'
import ConnectWalletButton from '../components/ConnectWalletButton'
import TodoList from '../components/TodoList'
import { TaskContractAddress } from '../config'
import TaskAbi from '../../backend/build/contracts/Taskcontract.json'
import { ethers } from 'ethers'
import { useState } from 'react'
import React, { useEffect } from 'react';

/* 
const tasks = [
  { id: 0, taskText: 'clean', isDeleted: false }, 
  { id: 1, taskText: 'food', isDeleted: false }, 
  { id: 2, taskText: 'water', isDeleted: true }
]
*/

export default function Home() {
  const [correctNetwork, setCorrectNetwork] = useState(false)
  const [isUserLoggedIn, setIsUserLoggedIn] = useState(false)
  const [currentAccount, setCurrentAccount] = useState('')
  const [input, setInput] = useState('')
  const [transactions, setTransactions] = useState([])

  useEffect(() => {
    connectWallet()
    getAllTransactions()
  }, [])
  // Calls Metamask to connect wallet on clicking Connect Wallet button
  const connectWallet = async () => {
    try {
      const { ethereum } = window
      if (!ethereum) {
        console.log("Metamark not detected")
        return
      }

      let chainId = await ethereum.request({ method: 'eth_chainId' })
      console.log("conneted to chain ", chainId)

      setCorrectNetwork(true)

      const accounts = await ethereum.request({ method: 'eth_requestAccounts' })

      console.log('Found accounts', accounts[0])
      setIsUserLoggedIn(true)
      setCurrentAccount(accounts[0])
    } catch (error) {
      console.log(error)
    }
  }

  // Just gets all the tasks from the contract
  const getAllTransactions = async () => {
    try {
      const { ethereum } = window
      if (ethereum) {
        const provider = new ethers.providers.Web3Provider(ethereum)
        const signer = provider.getSigner()
        const TaskContract = new ethers.Contract(
          TaskContractAddress,
          TaskAbi.abi,
          signer
        )
        let allTransactions = await TaskContract.getMyTransactions()
        setTasks(allTransactions)
      } else {
        console.log('ethereum does not exist')
      }
    } catch (error) {
      console.log(error)
    }
  }

  // Add tasks from front-end onto the blockchain
  const addTask = async e => {
    e.preventDefault()

    let task = {
      taskText: input,
      isDeleted: false
    }

    try {
      const { ethereum } = window
      if (ethereum) {
        const provider = new ethers.providers.Web3Provider(ethereum)
        const signer = provider.getSigner()
        const TaskContract = new ethers.Contract(
          TaskContractAddress,
          TaskAbi.abi,
          signer
        )
        // console.log("ffgfhj")
        // console.log(TaskAbi.abi)
        TaskContract.addTask(task.taskText, task.isDeleted)

          .then(res => {
            setTasks([...tasks, task])
            console.log("Added")
          })
          .catch(err => {
            console.log(err)
          })
      } else {
        console.log("Ethereum object doesn't exist")
      }
    } catch (error) {
      console.log(error)
    }

    setInput('')
  }

  return (
    <div className='bg-[#97b5fe] h-screen w-screen flex justify-center py-6'>
      {!isUserLoggedIn ? <ConnectWalletButton connectWallet={connectWallet} /> :
        correctNetwork ? <TodoList tasks={tasks} input={input} setInput={setInput} addTask={addTask} deleteTask={deleteTask} /> : <WrongNetworkMessage />}
    </div>
  )
}

