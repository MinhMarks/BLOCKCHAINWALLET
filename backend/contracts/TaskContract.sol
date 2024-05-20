pragma solidity 0.8.15;

contract Election{
  struct Candidate
  {
  uint id;
  string name;
  uint votecount;
  }
//Storing the accounts that have voted.
  mapping(address => bool) public voters;
  //Store and fetch candidate
  mapping(uint => Candidate) public candidates;
  //Storing the candidate count 
  uint public candidatesCount;
  //voted event
  event votedEvent(  uint indexed _candidateId);
  
  // constructor() public
  // {
  //   addCandidate("BJP");
  //   addCandidate("Congress");
  //   addCandidate("Aam Aadmi Party");
  // }
  function addCandidate(string memory _name) private
  {
    candidatesCount++;
    candidates[candidatesCount] = Candidate(candidatesCount, _name, 0);
  }
  
  function vote(uint _candidateId) public
  {
    //require that they haven't voted before
    require(!voters[msg.sender]);
    //requie a valid candidate
    require(_candidateId > 0 && _candidateId <= candidatesCount);
    //record that voter has voted
    voters[msg.sender] = true;
    //Update candidate vote count
    candidates[_candidateId].votecount++;
    //trigger voted event
    emit votedEvent(_candidateId);
  }
}